#pragma once
#include <core/Mesh.h>

/// @brief Genera una mesh de tipo cubo
class CubeMesh : public Mesh
{
public:
	CubeMesh();
	~CubeMesh() {}
	CubeMesh(Shader* shader);

	void generateMesh() override;
};

