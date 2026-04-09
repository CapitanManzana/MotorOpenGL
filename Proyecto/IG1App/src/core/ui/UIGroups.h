#pragma once
#include <cstdint>
#define UI_WINDOW_GROUP_LIST INSPECTOR, VIEWPORT, SCENE, CONSOLE, LIGHTING, NUM_GROUP

namespace cme::ui {
	using windowGroupID_t = uint8_t;
	/// @brief Clasificación de los distintos grupos de ventanas que hay en la interfaz, se pueden usar para acceder a las ventanas desde el UIManager
	enum windowGroupID : windowGroupID_t
	{
		None = 0,
		UI_WINDOW_GROUP_LIST
	};

	template<typename T>
	constexpr windowGroupID_t getWindowID = T::id;
}

#define WINDOW_ID(cId) constexpr static ui::windowGroupID_t id = cId;