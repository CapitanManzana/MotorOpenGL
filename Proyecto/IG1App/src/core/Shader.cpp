#include <glad/glad.h>
#include "Shader.h"
#include <fstream>
#include <iostream>
#include <utils/logger.h>
#include <cassert>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cme {
	Shader::Shader(ShaderCreationData ctx) {
		std::string vertShaderSource = getShaderSource(ctx.vertexShaderPath);
		std::string fragShaderSource = getShaderSource(ctx.fragmentShaderPath);

		GLuint vertShader = compileShader(vertShaderSource.c_str(), GL_VERTEX_SHADER);
		GLuint fragShader = compileShader(fragShaderSource.c_str(), GL_FRAGMENT_SHADER);

		_shaderProgram = glCreateProgram();
		glAttachShader(_shaderProgram, vertShader);
		glAttachShader(_shaderProgram, fragShader);
		glLinkProgram(_shaderProgram); // Se linkea el programa

		// Comprobar si hay errores de linker
		int success;
		char infoLog[512];
		glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
			LOG_ERROR("Linking shader failed\n" << infoLog);
		}

		// Hay que borrar los shaders ya que ya se han compilado y linkeado
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}

	void Shader::use() const {
		glUseProgram(_shaderProgram);
	}

	std::string Shader::getShaderSource(const std::string& shaderPath) const {
		// std::ios::ate pone el cursor al final para poder saber el tamaño total
		std::ifstream sFile(shaderPath, std::ios::ate | std::ios::binary);

		if (!sFile.is_open()) {
			LOG_ERROR("SHADER::FILE_NOT_SUCCESFULLY_READ: " << shaderPath);
			return "";
		}

		size_t fileSize = (size_t)sFile.tellg();
		std::string shaderSrc(fileSize, ' '); // Reservamos la memoria exacta de golpe

		sFile.seekg(0); // Volvemos al inicio del archivo
		sFile.read(&shaderSrc[0], fileSize); // Leemos directo a la memoria del string
		sFile.close();

		return shaderSrc;
	}

	GLuint Shader::compileShader(const char* shaderSource, GLenum shaderType) const {
		GLuint id;
		id = glCreateShader(shaderType);
		glShaderSource(id, 1, &shaderSource, NULL);
		glCompileShader(id);

		int success;
		char infoLog[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::string tipoStr = (shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
			LOG_ERROR("Compiling " << tipoStr << " shader failed\n" << infoLog);

			return 0;
		}

		return id;
	}

	void Shader::setUniform(const std::string& name, float value) {
		int loc = glGetUniformLocation(_shaderProgram, name.c_str());
		glUniform1f(loc, value);
	}

	void Shader::setUniform(const std::string& name, const glm::vec3& value) {
		int loc = glGetUniformLocation(_shaderProgram, name.c_str());
		glUniform3f(loc, value.r, value.g, value.b);
	}

	void Shader::setUniform(const std::string& name, const glm::vec4& value) {
		int loc = glGetUniformLocation(_shaderProgram, name.c_str());
		glUniform4f(loc, value.r, value.g, value.b, value.a);
	}

	void Shader::setUniform(const std::string& name, const glm::mat4& value) {
		int loc = glGetUniformLocation(_shaderProgram, name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}
}