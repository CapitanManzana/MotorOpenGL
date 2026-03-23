#pragma once
#include "component.h"
#include <utility>
#include <vector>
#include <type_traits>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <core/serialize/Serializable.h>
#include <core/serialize/JsonSerializer.h>

class Scene;
namespace ec
{
	class EntityManager;
	class Entity : public capiEngine::Serializable
	{
	public:
		Entity(ent::groupID groupID, Scene* _scenePtr, std::string name)
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
		Scene* _scene;

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

		void serialize(capiEngine::JsonSerializer& s) const {
			
		}

		void deserialize(capiEngine::JsonSerializer& s) {
			
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
			component->setContext(this);

			// Inicializar el componente
			component->initComponent();

			// Devolver el componente
			return component;
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

		inline Scene* getScene() {
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
