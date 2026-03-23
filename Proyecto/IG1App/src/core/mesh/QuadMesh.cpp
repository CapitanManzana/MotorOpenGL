#include "QuadMesh.h"
#include <core/Shader.h>

QuadMesh::QuadMesh() {
	generateMesh();
	initBuffers();

	_id = capiEngine::QUAD;
}

QuadMesh::QuadMesh(Shader* shader) : QuadMesh() {
	_shader = shader;
}

void QuadMesh::generateMesh() {
	_vertices.emplace_back(-0.5f, -0.5f, 0);
	_vertices.emplace_back(0.5f, -0.5f, 0);
	_vertices.emplace_back(0.5f, 0.5f, 0);
	_vertices.emplace_back(-0.5f, 0.5f, 0);

	_indices.emplace_back(0, 1, 3);
	_indices.emplace_back(1, 2, 3);

	_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
	_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
	_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
	_vColor.emplace_back(1.0f, 0.0f, 1.0f, 1.0f);
}