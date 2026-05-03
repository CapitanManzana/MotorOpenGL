#include <mesh/QuadMesh.h>
#include <surface/Shader.h>
#include <surface/Material.h>

namespace cme {
	QuadMesh::QuadMesh() {
		generateMesh();
		initBuffers();
		_mat = new Material();

		_id = QUAD;
	}

	QuadMesh::QuadMesh(Shader* shader) : QuadMesh() {
		_mat->setShader(shader);
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
}