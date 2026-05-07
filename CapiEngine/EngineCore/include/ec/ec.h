#pragma once
#include <cstdint> // wtf
#include <memory>
#define _CMPS_LIST_ TRANSFORM, MESH_RENDERER, LIGHT, LUA_SCRIPT, NUM_COMPONENTS
#define _GRPS_LIST_ TRANSPARENT, NUM_GROUP

namespace ec
{
	class Entity;
	class Component;
	using entity_t = std::shared_ptr<Entity>;

	// Para ahorrar, no hay tantos componentes
	using cmpID_t = uint8_t;
	namespace comp
	{
		enum cmpID : cmpID_t
		{
			DEFAULT = 0,
			_CMPS_LIST_
		};
		constexpr cmpID_t maxComponentId = NUM_COMPONENTS;
	}

	// Dado un componente, devolver su ID que corresponde al enum cmpID
	// Para ello el componente tiene que estar "registrado"
	// El tipo es uint8_t, el mismo para el cmpID
	template<typename T>
	constexpr cmpID_t getComponentID = T::id;


	using groupID_t = uint8_t;
	namespace ent
	{
		enum groupID : groupID_t
		{
			None = 0,
			_GRPS_LIST_
		};
		constexpr groupID_t maxGroupLayer = NUM_GROUP;
	}

	class RenderComponent;
	class UpdateComponent;

	template<typename T>
	inline RenderComponent* tryGetAsRender(T* self) {
		if constexpr (std::is_base_of_v<RenderComponent, T>)
			return static_cast<RenderComponent*>(self);
		return nullptr;
	}

	template<typename T>
	inline UpdateComponent* tryGetAsUpdate(T* self) {
		if constexpr (std::is_base_of_v<UpdateComponent, T>)
			return static_cast<UpdateComponent*>(self);
		return nullptr;
	}
}

#define __CMPID_DECL__(cId) constexpr static ec::cmpID_t id = cId; \
	ec::cmpID_t getID() const override { return id; } \
	ec::RenderComponent* getAsRender() override { return ec::tryGetAsRender(this); } \
    ec::UpdateComponent* getAsUpdate() override { return ec::tryGetAsUpdate(this); }