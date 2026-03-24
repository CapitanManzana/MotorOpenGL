#pragma once
#include <cstdint>
#include <array>

#define MESH_TYPES_ID TRIANGLE, QUAD, CUBE
#define MESH_TYPES_NAMES "TRIANGLE", "QUAD", "CUBE"

namespace cme {
	using meshID_t = uint8_t;
	enum meshID : meshID_t
	{
		None = 0,
		MESH_TYPES_ID,
		MESH_COUNT
	};

	const std::array<const char*, MESH_COUNT> MESH_T_NAMES = { "None", MESH_TYPES_NAMES};
}