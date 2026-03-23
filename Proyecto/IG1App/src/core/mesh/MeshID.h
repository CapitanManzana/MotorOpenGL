#pragma once
#include <cstdint>

namespace capiEngine {
	using meshID_t = uint8_t;
	enum meshID : meshID_t
	{
		None = 0,
		TRIANGLE,
		QUAD,
		CUBE,
		MESH_COUNT
	};
}