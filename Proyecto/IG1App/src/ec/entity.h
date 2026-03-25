#pragma once
#include "component.h"
#include <utility>
#include <vector>
#include <type_traits>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <core/serialize/JsonSerializer.h>
#include <core/serialize/Serializable.h>
#include <core/register/ComponentRegistry.h>

namespace cme {
	class Scene;
}

namespace ec
{
	class EntityManager;
	class Entity : public cme::Serializable, public std::enable_shared_from_this<Entity>
	{
	public:
		Entity(ent::groupID groupID, cme::Scene* _scenePtr, std::string name)
			: _alive(true)
			, _active(true)
			, _groupID(ec::ent::None)
			, _scene(_scenePtr)
			, _updateComponents(0, nullptr)
			, _renderComponents(0, nullptr)
			, _components(ec::comp::NUM_COMPONENTS, nullptr)
			, _name(name)
		{
#ifdef _DEBUG
			LOG_INFO("Creating entity");
#endif // _DEBUG
			_groupID = groupID;
		};

		/// @brief Destructor virtual, destruira todos los componentes que hay tiene la entidad
		virtual ~Entity()
		{
			for (auto c : _components) { delete c; }
			_updateComponents.clear();
			_renderComponents.clear();
			_components.clear();
		}

		/// @brief No se permite duplicar un entidad
		Entity(const Entity&) = delete;

		/// @brief No se permite "asignar" una entidad a otra entidad
		Entity& operator=(const Entity&) = delete;

	protected:
		/// @brief El 
		ec::ent::groupID _groupID;

	private:
		bool _alive;
		bool _active;
		cme::Scene* _scene;

		std::string _name;
	public:
		/// @breif componentes que tienen update 
		std::vector<UpdateComponent*> _updateComponents;

		/// @breif componentes que tienen render
		std::vector<RenderComponent*> _renderComponents;

		/// @brief vector del todos los componentes
		std::vector<Component*> _components;

		std::string& name() { return _name; }

		inline void update() {
			for (auto* comp : _updateComponents) {
				comp->update();
			}
		}

		void render() {
			for (auto* comp : _renderComponents) {
				comp->render();
			}
		}

		void serialize(cme::JsonSerializer& s) const {
			// 1. Añadimos una nueva entidad al array de entidades
			s.pushObjectToArray();
			// 2. Guardamos sus datos básicos
			s.write("name", this->_name); // Asumiendo que tu entidad tiene nombre
			// 3. Abrimos el array de componentes
			s.beginArray("components");
			for (auto component : _components) {
				if (!component) continue;
				// Por cada componente, metemos un nuevo objeto en el array
				s.pushObjectToArray();
				s.write("type", component->serializeID());

				s.beginScope("data");
				component->serialize(s); // Aquí llama al Transform, MeshRenderer, etc.
				s.endScope();

				s.endScope(); // Cerramos el objeto de este componente
			}

			s.endScope(); // Cerramos el array "components"
			s.endScope(); // Cerramos el objeto de la entidad
		}

		void deserialize(cme::JsonSerializer& s) {
			s.beginArray("components");
			size_t numComp = s.getArraySize();
			for (size_t k = 0; k < numComp; k++) {
				s.enterElement(k);
				std::string compKey = s.readString("type");
				auto comp = cme::ComponentRegistry::create(compKey);
				if (!comp) {
					LOG_ERROR(std::format("El componente es nulo, la key no existe. KEY: {}", compKey));
					continue;
				}

				this->addComponent(comp);
				s.beginScope("data");
				comp->deserialize(s);
				s.endScope();

				s.endScope();
			}
			s.endScope(); // Salimos de los componentes
		}
		
		/// @brief Añadir un componente a la entidad
		/// @tparam T el componente
		/// @tparam ...Ts los parametros para inicializar el componente
		/// @param component
		/// @param ...args argumentos para la construccion del componente T
		/// @return T el componente
		template<typename T, typename... Ts>
		inline T* addComponent(Ts&&... args)
		{
			// Comprobar en tiempo de ejecucion si T es un componente
			static_assert(std::is_base_of<Component, T>::value, "No es un componente"); // La putada es que no te dice donde has puesto mal...

			// Obtener el ID del componente T, si no esta registrado 
			constexpr comp::cmpID id = static_cast<comp::cmpID>(ec::getComponentID<T>);

			// Quitar el componente
			removeComponent<T>();

			// Crear el componente y asignarlo en el vector del componentes
			T* component = new T(std::forward<Ts>(args)...);
			_components[id] = component;

			//se comprueba si es un componente de render para meterlo en el vector correspondiente
			if constexpr (std::is_base_of_v<RenderComponent, T>) {
				_renderComponents.push_back(static_cast<RenderComponent*>(component));
			}

			if constexpr (std::is_base_of_v<UpdateComponent, T>) {
				_updateComponents.push_back(static_cast<UpdateComponent*>(component));
				component->setUpdateIterator(std::prev(_updateComponents.end()));
			}


			// Asignar el owner del componente
			component->setContext(shared_from_this());

			// Inicializar el componente
			component->initComponent();

			// Devolver el componente
			return component;
		}

		/// @brief Añade un componente ya creado a la entidad, para Deserialización
		/// @param component El puntero al componente ya instanciado
		void addComponent(ec::Component* component) {
			if (!component) return;

			ec::cmpID_t id = component->getID();

			// Borrar si ya existe
			delete _components[id];
			_components[id] = component;

			// Registrar en render/update si aplica
			if (auto* r = component->getAsRender()) {
				_renderComponents.push_back(r);
			}
			if (auto* u = component->getAsUpdate()) {
				_updateComponents.push_back(u);
				u->setUpdateIterator(std::prev(_updateComponents.end()));
			}

			component->setContext(shared_from_this());
			component->initComponent();
		}

		/// @brief Quitar un componente de la entidad
		/// @tparam T El componente a quitar
		template<typename T>
		inline void removeComponent()
		{
			// Comprobar en tiempo de ejecucion si T es un componente
			static_assert(std::is_base_of<Component, T>::value, "No es un componente");

			// Obtener el ID del componente T, si no esta registrado
			constexpr comp::cmpID id = static_cast<comp::cmpID>(ec::getComponentID<T>);

			auto component = _components[id];


			/*
				Aqui deberia poder comprobar que componente especial es, y borrarlo de su lista correspondiente mediante el iterador, pero no me sale...
			*/

			//if constexpr (std::is_base_of_v<UpdateComponent, T>)
			//{
			//	static_cast<UpdateComponent*>(component);
			//	_updateComponents.erase(component->getUpdateIterator());
			//}

			delete _components[id];
			_components[id] = nullptr;
		}

		/// @brief Ver si la entidad tiene un componente T
		/// @tparam T El componente a comprobar
		/// @return true si existe, false si no existe
		template<typename T>
		inline bool hasComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "No es un componente");
			constexpr comp::cmpID id = static_cast<comp::cmpID>(ec::getComponentID<T>);
			return _components[id] != nullptr;
		}

		/// @brief Coger el componente T de la entidad
		/// @tparam T El componente a coger
		/// @return T* el componete si existe, nullptr si no existe 
		template<typename T>
		inline T* getComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "No es un componente");
			constexpr comp::cmpID id = static_cast<comp::cmpID>(ec::getComponentID<T>);
			return static_cast<T*>(_components[id]);
		}

		/// @brief Setter del grupo al que pertenece la entidad
		/// @param el id al que 
		inline void setGroup(ec::ent::groupID id) { _groupID = id; }

		inline cme::Scene* getScene() {
			return _scene;
		}

		/// @brief Getter de si el componente esta "vivo"
		/// @return 
		bool isAlive() { return _alive; }

		/// @brief Getter si el componente esta activo
		/// @return 
		bool& active() { return _active; }

		/// @brief Getter del la lista de componentes que tiene update
		/// @return la lista de componentes que tiene update
		std::vector<UpdateComponent*>& getUpdateComponent() {
			return _updateComponents;
		}

		/// @brief Getter del la lista de componentes que tiene render
		/// @return la lista de componentes que tiene render
		std::vector<RenderComponent*>& getRenderComponent() {
			return _renderComponents;
		}
	};
}; // namespace ec
