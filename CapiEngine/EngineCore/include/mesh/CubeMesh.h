#pragma once
#include <mesh/Mesh.h>

namespace cme {
	/// @brief Genera una mesh de tipo cubo
	class CubeMesh : public Mesh
	{
	public:
		CubeMesh();
		~CubeMesh() {}
		CubeMesh(Shader* shader);

		void generateMesh() override;
	};
}
