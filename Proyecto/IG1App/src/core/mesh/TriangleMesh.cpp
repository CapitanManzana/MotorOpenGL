#include "TriangleMesh.h"
#include <core/Shader.h>
#include <core/Material.h>

namespace cme {
	TriangleMesh::TriangleMesh() {
		generateMesh();
		initBuffers();
		_mat = new Material();

		_id = TRIANGLE;
	}

	TriangleMesh::TriangleMesh(Shader* shader) {
		_shader = shader;
		generateMesh();
		initBuffers();

		_id = TRIANGLE;
	}

	void TriangleMesh::generateMesh() {
		_vertices.emplace_back(-0.5f, -0.5f, 0);
		_vertices.emplace_back(0.5f, -0.5f, 0);
		_vertices.emplace_back(0.0f, 0.5f, 0);

		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
	}
}