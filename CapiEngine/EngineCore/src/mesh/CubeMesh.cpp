#include "mesh/CubeMesh.h"
#include <surface/Shader.h>
#include <surface/Material.h>

namespace cme {
	CubeMesh::CubeMesh() {
		generateMesh();
		initBuffers();
		_mat = new Material();

		_id = CUBE;
	}

	CubeMesh::CubeMesh(Shader* shader) : CubeMesh() {
		_mat->setShader(shader);
	}

	void CubeMesh::generateMesh() {
		float l = 0.5f;

		// --- Cara Frontal (+Z) ---
		_vertices.emplace_back(-l, -l, l); // 0: Abajo-Izq
		_vertices.emplace_back(l, -l, l); // 1: Abajo-Der
		_vertices.emplace_back(l, l, l); // 2: Arriba-Der
		_vertices.emplace_back(-l, l, l); // 3: Arriba-Izq
		_indices.emplace_back(0, 1, 3);
		_indices.emplace_back(1, 2, 3);

		// --- Cara Superior (+Y) ---
		_vertices.emplace_back(-l, l, l); // 4: Abajo-Izq (Viendo desde arriba)
		_vertices.emplace_back(l, l, l); // 5: Abajo-Der
		_vertices.emplace_back(l, l, -l); // 6: Arriba-Der
		_vertices.emplace_back(-l, l, -l); // 7: Arriba-Izq
		_indices.emplace_back(4, 5, 7);
		_indices.emplace_back(5, 6, 7);

		// --- Cara Trasera (-Z) ---
		// Ojo: Se definen de derecha a izquierda para mantener el sentido antihorario viéndolo desde atrás
		_vertices.emplace_back(l, -l, -l); // 8: Abajo-Izq (Viendo desde atrás)
		_vertices.emplace_back(-l, -l, -l); // 9: Abajo-Der
		_vertices.emplace_back(-l, l, -l); // 10: Arriba-Der
		_vertices.emplace_back(l, l, -l); // 11: Arriba-Izq
		_indices.emplace_back(8, 9, 11);
		_indices.emplace_back(9, 10, 11);

		// --- Cara Inferior (-Y) ---
		_vertices.emplace_back(-l, -l, -l); // 12: Abajo-Izq (Viendo desde abajo)
		_vertices.emplace_back(l, -l, -l); // 13: Abajo-Der
		_vertices.emplace_back(l, -l, l); // 14: Arriba-Der
		_vertices.emplace_back(-l, -l, l); // 15: Arriba-Izq
		_indices.emplace_back(12, 13, 15);
		_indices.emplace_back(13, 14, 15);

		// --- Cara Derecha (+X) ---
		_vertices.emplace_back(l, -l, l); // 16: Abajo-Izq (Viendo desde la derecha)
		_vertices.emplace_back(l, -l, -l); // 17: Abajo-Der
		_vertices.emplace_back(l, l, -l); // 18: Arriba-Der
		_vertices.emplace_back(l, l, l); // 19: Arriba-Izq
		_indices.emplace_back(16, 17, 19);
		_indices.emplace_back(17, 18, 19);

		// --- Cara Izquierda (-X) ---
		_vertices.emplace_back(-l, -l, -l); // 20: Abajo-Izq (Viendo desde la izquierda)
		_vertices.emplace_back(-l, -l, l); // 21: Abajo-Der
		_vertices.emplace_back(-l, l, l); // 22: Arriba-Der
		_vertices.emplace_back(-l, l, -l); // 23: Arriba-Izq
		_indices.emplace_back(20, 21, 23);
		_indices.emplace_back(21, 22, 23);


		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);

		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);

		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
		_vColor.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);

		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
		_vColor.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);

		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);

		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
		_vColor.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);

		_normals.emplace_back(0, 0, 1);
		_normals.emplace_back(0, 0, 1);
		_normals.emplace_back(0, 0, 1);
		_normals.emplace_back(0, 0, 1);

		// --- Cara Superior (+Y) ---
		_normals.emplace_back(0, 1, 0);
		_normals.emplace_back(0, 1, 0);
		_normals.emplace_back(0, 1, 0);
		_normals.emplace_back(0, 1, 0);

		// --- Cara Trasera (-Z) ---
		_normals.emplace_back(0, 0, -1);
		_normals.emplace_back(0, 0, -1);
		_normals.emplace_back(0, 0, -1);
		_normals.emplace_back(0, 0, -1);

		// --- Cara Inferior (-Y) ---
		_normals.emplace_back(0, -1, 0);
		_normals.emplace_back(0, -1, 0);
		_normals.emplace_back(0, -1, 0);
		_normals.emplace_back(0, -1, 0);

		// --- Cara Derecha (+X) ---
		_normals.emplace_back(1, 0, 0);
		_normals.emplace_back(1, 0, 0);
		_normals.emplace_back(1, 0, 0);
		_normals.emplace_back(1, 0, 0);

		// --- Cara Izquierda (-X) ---
		_normals.emplace_back(-1, 0, 0);
		_normals.emplace_back(-1, 0, 0);
		_normals.emplace_back(-1, 0, 0);
		_normals.emplace_back(-1, 0, 0);

		for (int i = 0; i < 6; ++i) {
			_texCoords.emplace_back(0.0f, 0.0f); // Abajo-Izq
			_texCoords.emplace_back(1.0f, 0.0f); // Abajo-Der
			_texCoords.emplace_back(1.0f, 1.0f); // Arriba-Der
			_texCoords.emplace_back(0.0f, 1.0f); // Arriba-Izq
		}
	}
}