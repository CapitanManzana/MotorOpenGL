#pragma once

#include <iostream>
#include "ec.h"
#include <functional>
#include <array>
#include <utils/logger.h>
#include <serialize/Serializable.h>

class Transform;
class Scene;
namespace ec
{
	class RenderComponent;
	class UpdateComponent;

	/// @brief Declaracion adelantada, para evitar dependencia circular
	class Component : public cme::Serializable
	{
		using Iterator = std::vector<Component*>::iterator;
	public:

		/// @brief Constructor
		Component() {
#ifdef _DEBUG
			LOG_INFO("Creating base component");
#endif // _DEBUG
		};

		/// @brief Destructor
		virtual ~Component() { };

		protected:
			std::weak_ptr<ec::Entity> _entity;
			Iterator _iterator;

	public:

		/// @brief Hacer saber al componente quien le controla
		/// @param ent La entidad que la controla
		inline void setContext(std::weak_ptr<ec::Entity> ent)
		{
			_entity = ent;
		}

		inline virtual void initComponent() {}

		inline virtual void start() {}
		virtual ec::cmpID_t getID() const = 0;
		virtual std::string serializeID() const = 0;

		virtual void serialize(cme::JsonSerializer& s) const override {}
		virtual void deserialize(cme::JsonSerializer& s) override {}

		virtual RenderComponent* getAsRender() { return nullptr; }
		virtual UpdateComponent* getAsUpdate() { return nullptr; }
	};


	/// @brief Componente de render
	class RenderComponent {
		using RenderIterator = std::vector<RenderComponent*>::iterator;
	protected:
		RenderIterator _renderIterator;
	public:
		virtual ~RenderComponent() {}
		virtual void render() const = 0;
	};

	/// @brief Componente de update
	class UpdateComponent {
		using UpdateIterator = std::vector<UpdateComponent*>::iterator;
	protected:
		UpdateIterator _updateIterator;
	public:
		virtual void update() =  0;

		inline UpdateIterator& getUpdateIterator() { return _updateIterator; } // deberia haber aqui un assert
		void setUpdateIterator(UpdateIterator it) { _updateIterator = it; }
	};
}
