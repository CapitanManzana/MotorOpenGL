#include "Mesh.h"
#include <core/Shader.h>
#include <algorithm>

namespace cme {
	Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::uvec3>& indices)
		: _vertices(vertices), _indices(indices) 
	{
		_vColor.resize(_vertices.size(), glm::vec4(1.0f));
		initBuffers();
	}

	Mesh::~Mesh() {
		if (_VAO != 0) {
			glDeleteVertexArrays(1, &_VAO);
			glDeleteBuffers(1, &_VBO);
			glDeleteBuffers(1, &_CBO);
			if (_EBO != 0) glDeleteBuffers(1, &_EBO);
		}
	}

	void Mesh::setMeshData(const std::vector<glm::vec3>& vertices, const std::vector<glm::uvec3>& indices) {
		_vertices = vertices;
		_indices = indices;
		_vColor.assign(_vertices.size(), glm::vec4(1.0f));
		initBuffers();
	}

	void Mesh::setMeshData(const std::vector<glm::vec3>& vertices, const std::vector<glm::uvec3>& indices, const std::vector<glm::vec4>& colors) {
		_vertices = vertices;
		_indices = indices;
		_vColor = colors;
		if (_vColor.size() < _vertices.size()) {
			_vColor.resize(_vertices.size(), glm::vec4(1.0f));
		}
		initBuffers();
	}

	void Mesh::render() const {
		glBindVertexArray(_VAO);
		if (!_indices.empty()) {
			glDrawElements(mPrimitive, (GLsizei)_indices.size() * 3, GL_UNSIGNED_INT, 0);
		} else {
			glDrawArrays(mPrimitive, 0, (GLsizei)_vertices.size());
		}
		glBindVertexArray(0);
	}

	void Mesh::initBuffers() {
		if (_VAO == 0) glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		if (_VBO == 0) glGenBuffers(1, &_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), _vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		if (_CBO == 0) glGenBuffers(1, &_CBO);
		glBindBuffer(GL_ARRAY_BUFFER, _CBO);
		glBufferData(GL_ARRAY_BUFFER, _vColor.size() * sizeof(glm::vec4), _vColor.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(1);

		if (!_indices.empty()) {
			if (_EBO == 0) glGenBuffers(1, &_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(glm::uvec3), _indices.data(), GL_STATIC_DRAW);
		}

		glBindVertexArray(0);
	}

	void Mesh::getLocalAABB(glm::vec3& outMin, glm::vec3& outMax) const {
		if (_vertices.empty()) {
			outMin = outMax = glm::vec3(0.0f);
			return;
		}
		outMin = outMax = _vertices[0];
		for (const auto& v : _vertices) {
			outMin.x = std::min(outMin.x, v.x);
			outMin.y = std::min(outMin.y, v.y);
			outMin.z = std::min(outMin.z, v.z);
			outMax.x = std::max(outMax.x, v.x);
			outMax.y = std::max(outMax.y, v.y);
			outMax.z = std::max(outMax.z, v.z);
		}
	}
}
