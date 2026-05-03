#pragma once
#include <mesh/Mesh.h>

namespace cme {
	/// @brief Crea una mesh de tipo Quad
	class QuadMesh : public Mesh
	{
	public:
		QuadMesh();
		QuadMesh(Shader* shader);

		void generateMesh() override;
	};
}
