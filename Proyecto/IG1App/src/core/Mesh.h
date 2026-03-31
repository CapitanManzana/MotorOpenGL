#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <core/mesh/MeshID.h>

namespace cme {
	class Shader;

	/// @brief Clase que representa una malla (Geometry + Buffers)
	class Mesh
	{
	protected:
		Shader* _shader = nullptr;

		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec4> _vColor;
		std::vector<glm::uvec3> _indices;

		GLenum mPrimitive = GL_TRIANGLES;

		GLuint _VAO = 0;	// Vertex Array Object
		GLuint _VBO = 0;	// Vertex Buffer Object
		GLuint _CBO = 0;	// Color Buffer Object
		GLuint _EBO = 0;	// Element Bueffer Object

		glm::mat4 _model = glm::mat4(1.0f); 

		meshID _id = None;
	public:
		/// @brief Constructor por defecto
		Mesh() = default;
		/// @brief Constructor con datos
		Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::uvec3>& indices);
		virtual ~Mesh();

		/// @brief Renderiza el mesh
		virtual void render() const;

		/// @brief Metodo para generar la malla (si aplica en subclases)
		virtual void generateMesh() {}

		/// @brief Cambia el shader que utiliza la mesh
		void setShader(Shader* shader) { _shader = shader; }
		/// @brief Establece la matriz de modelado de la malla
		void setModelMatrix(glm::mat4 model) { _model = model; }
		
		glm::mat4 modelMatrix() { return _model; }
		Shader* shader() { return _shader; }
		meshID id() { return _id; }

		void getLocalAABB(glm::vec3& outMin, glm::vec3& outMax) const;

		void setMeshData(const std::vector<glm::vec3>& vertices, const std::vector<glm::uvec3>& indices);
		void setMeshData(const std::vector<glm::vec3>& vertices, const std::vector<glm::uvec3>& indices, const std::vector<glm::vec4>& colors);

		size_t getNumVertices() const { return _vertices.size(); }
		size_t getNumIndices() const { return _indices.size() * 3; }

	protected:
		void initBuffers();
	};
}
