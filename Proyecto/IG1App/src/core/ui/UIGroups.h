#pragma once
#include <cstdint>
#define UI_WINDOW_GROUP_LIST INSPECTOR, VIEWPORT, SCENE, CONSOLE, NUM_GROUP

namespace capiEngine::ui {
	using windowGroupID_t = uint8_t;
	enum windowGroupID : windowGroupID_t
	{
		None = 0,
		UI_WINDOW_GROUP_LIST
	};

	template<typename T>
	constexpr windowGroupID_t getWindowID = T::id;
}

#define WINDOW_ID(cId) constexpr static capiEngine::ui::windowGroupID_t id = cId;