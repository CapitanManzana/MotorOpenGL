#pragma once
#include <glad/glad.h>

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <mesh/MeshID.h>

namespace cme {
	class Shader;
	class Material;
	/// @brief Clase abstracta que sirve como padre para crear todo tipo de mallas distintas
	class Mesh
	{
	protected:
		Material* _mat = nullptr;

		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec4> _vColor;
		std::vector<glm::uvec3> _indices;
		std::vector<glm::vec3> _normals;
		std::vector<glm::vec2> _texCoords;

		GLuint mPrimitive = GL_TRIANGLES;

		GLuint _VAO = 0;	// Vertex Array Object
		GLuint _LightVAO = 0;	// Vertex Array Object
		GLuint _VBO = 0;	// Vertex Buffer Object
		GLuint _CBO = 0;	// Color Buffer Object
		GLuint _EBO = 0;	// Element Bueffer Object
		GLuint _NBO = 0;	// Normal Buffer Object
		GLuint _TBO = 0;	// Texture Buffer Object

		bool _isLightSource = false;

		glm::mat4 _model = glm::mat4(1.0f); // La matriz de modelado del mesh, que guarda su posici�n rotacion y escala en el mundo
		glm::mat3 _normalMatrix;

		meshID _id = None;
	public:
		/// @brief Constructor por defecto
		Mesh() = default;
		virtual ~Mesh();

		/// @brief Genera el mesh colocando los v�rtices, colores, etc...
		virtual void generateMesh() = 0;
		/// @brief Renderiza el mesh
		virtual void render() const;

		/// @brief Cambia el shader que utiliza la mesh
		/// @param shader El shader a poner
		void setMaterial(Material* mat) { _mat = mat; }
		/// @brief Establece la matriz de modelado de la malla
		/// @param model La nueva matriz
		void setModelMatrix(glm::mat4 model);
		/// @brief La matriz de modelado del mesh, que guarda su posici�n rotacion y escala en el mundo
		/// @return La matriz de modelado
		glm::mat4& modelMatrix() { return _model; }
		/// @brief La matriz de normales del mesh, que es la inversa de la transpuesta de la matriz 3x3 superior izquierda de la de modelado
		/// @return La matriz de normales
		glm::mat3& normalMatrix() { 
			calculateNormalMatrix();
			return _normalMatrix; 
		}

		Material* material() { return _mat; }

		meshID id() { return _id; }

		void getLocalAABB(glm::vec3& outMin, glm::vec3& outMax) const;

		bool isLightSource() const { return _isLightSource; }
		void setLightSource(bool value);
	protected:
		void initBuffers();

		void calculateNormalMatrix();
	};
}