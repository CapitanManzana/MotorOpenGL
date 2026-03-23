#include "TriangleMesh.h"
#include <core/Shader.h>

TriangleMesh::TriangleMesh() {
	generateMesh();
	initBuffers();

	_id = capiEngine::TRIANGLE;
}

TriangleMesh::TriangleMesh(Shader* shader) : TriangleMesh() { 
	_shader = shader;
}

void TriangleMesh::generateMesh() {
	_vertices.emplace_back(-0.5f, -0.5f, 0);
	_vertices.emplace_back(0.5f, -0.5f, 0);
	_vertices.emplace_back(0.0f, 0.5f, 0);

	_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
	_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
	_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
}