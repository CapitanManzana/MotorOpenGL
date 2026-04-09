#pragma once
#include <string>
#include <glm/fwd.hpp>
#include <unordered_map>
#include <glad/glad.h>

namespace cme {

	using RawUniform = std::vector<std::pair<std::string, GLenum>>;

	/// @brief Contiene la información necesaria para crear un shader, incluyendo las rutas de los archivos de shader de vértices y fragmentos. 
	/// Esta estructura se utiliza para encapsular los datos necesarios para la creación de un shader en la aplicación.
	struct ShaderCreationData {
		/// @brief Path del archivo del shader de vértices. Este archivo contiene el código fuente del shader de vértices que se utilizará para procesar los vértices en la aplicación.
		std::string vertexShaderPath;
		/// @brief Path del archivo del shader de fragmentos. Este archivo contiene el código fuente del shader de fragmentos que se utilizará para procesar los fragmentos (píxeles) en la aplicación.
		std::string fragmentShaderPath;
		/// @brief El nombre del archivo
		std::string name;

		ShaderCreationData() = default;
		ShaderCreationData(const std::string& vertexPath, const std::string& fragmentPath, const std::string& name)
			: vertexShaderPath(vertexPath), fragmentShaderPath(fragmentPath), name(name) {
		}

		/// @brief Comprueba si la data es valida, que ninguno de los 2 paths esten vacios
		/// @return Devuelve true si es valido
		bool isValid() const {
			return !vertexShaderPath.empty() && !fragmentShaderPath.empty();
		}
	};

	enum class ShaderType { UNLIT, LIT_PHONG};

	/// @brief Clase que representa un shader de OpenGL. Se encarga de cargar, compilar y gestionar los shaders en la aplicación.
	class Shader
	{
	private:
		// El programa con los shaders cargados
		GLuint _shaderProgram;
		std::string _name;

		std::unordered_map<std::string, int> _uniformLocationCache;
		ShaderType _type;
	public:
		/// @brief Constructor que carga el shader desde un archivo. El shader se compila y se prepara para su uso. 
		/// @param ctx Contexto para la cración de los shaders
		Shader(ShaderCreationData ctx);

		/// @brief Destruye el shader y libera los recursos asociados.
		virtual ~Shader() {}

		// cannot copy/move
		Shader(Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(Shader&) = delete;
		Shader& operator=(Shader&&) = delete;

		/// @brief Activa el shader para su uso en la aplicación. Esto hace que el shader sea el programa de shader activo en OpenGL.
		void use() const;

		RawUniform getActiveUniforms();

		/// @brief Establece un valor a una variable uniform del shader
		/// @param name Nombre de la variable
		/// @param value Valor de la variable
		void setUniform(const std::string& name, float value);
		void setUniform(const std::string& name, int value);
		void setUniform(const std::string& name, bool value);
		void setUniform(const std::string& name, const glm::vec2& value);
		void setUniform(const std::string& name, const glm::vec3& value);
		void setUniform(const std::string& name, const glm::vec4& value);
		void setUniform(const std::string& name, const glm::mat4& value);
		void setUniform(const std::string& name, const glm::mat3& value);

		std::string getName() { return _name; }
		ShaderType type() { return _type; }
	private:
		/// @brief Se encarga de leer el código fuente del shader desde un archivo. Dado un path al archivo del shader, esta función lee su contenido y devuelve el código fuente como una cadena de caracteres.
		/// @param shaderPath Donde se encuentra el archivo del shader que se desea cargar.
		/// @return Devuelve el código fuente del shader.
		std::string getShaderSource(const std::string& shaderPath) const;

		/// @brief Compila el shader a partir del código fuente proporcionado. Dado el código fuente del shader y su tipo (vértice o fragmento), esta función compila el shader y devuelve su ID de OpenGL.
		/// @param shaderSource El código fuente del shader que se desea compilar.
		/// @param shaderType El tipo de shader (por ejemplo, GL_VERTEX_SHADER o GL_FRAGMENT_SHADER) que se desea compilar.
		/// @return Devuelve el ID del shader compilado en OpenGL.
		GLuint compileShader(const char* shaderSource, GLenum shaderType) const;

		int getUniformLocation(const std::string& name);

	};
}
