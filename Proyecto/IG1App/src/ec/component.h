#pragma once

#include <iostream>
#include "ec.h"
#include <functional>
#include <array>
#include <utils/logger.h>

class Transform;
class Scene;
namespace ec
{
	/// @brief Declaracion adelantada, para evitar dependencia circular
	class Component
	{
		using Iterator = std::vector<Component*>::iterator;
		public:

		/// @brief Constructor
		Component() : _entity(nullptr) {
#ifdef _DEBUG
			LOG_INFO("Creating base component");
#endif // _DEBUG
		};

		/// @brief Destructor
		virtual ~Component() { };

		protected:
			entity_t _entity;
			Iterator _iterator;

	public:

		/// @brief Hacer saber al componente quien le controla
		/// @param ent La entidad que la controla
		inline void setContext(ec::entity_t ent)
		{
			_entity = ent;
		}

		inline virtual void initComponent() {}

		inline virtual void start() {}

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
