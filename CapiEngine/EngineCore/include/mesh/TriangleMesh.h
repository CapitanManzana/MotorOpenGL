#pragma once
#include <core/Mesh.h>

namespace cme {
	class Shader;

	/// @brief Genera el mesh de un triangulo
	class TriangleMesh : public Mesh
	{
	public:
		TriangleMesh();
		TriangleMesh(Shader* shader);

		void generateMesh() override;
	};
}
