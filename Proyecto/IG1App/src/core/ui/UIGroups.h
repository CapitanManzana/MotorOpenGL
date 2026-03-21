#pragma once
#include <cstdint>
#define UI_WINDOW_GROUP_LIST INSPECTOR, NUM_GROUP

namespace capiEngine::ui {
	using groupID_t = uint8_t;
	enum groupID : groupID_t
	{
		None = 0,
		UI_WINDOW_GROUP_LIST
	};

	template<typename T>
	constexpr groupID_t getWindowID = T::id;

}

#define WINDOW_ID(cId) constexpr static capiEngine::ui::groupID_t id = cId;