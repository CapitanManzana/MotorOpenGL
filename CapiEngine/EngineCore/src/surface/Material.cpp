#include <glad/glad.h>

#include "surface/Material.h"
#include "surface/Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <managers/ResourceManager.h>
#include <surface/Texture.h>

#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <lighting/GlobalLight.h>

#include <serialize/JsonSerializer.h>
#include <managers/LightManager.h>

namespace cme {
	const std::unordered_set<std::string> Material::CAMERA_UNIFORMS = {
			"projection", "modelView", "model", "normalMatrix",
			"cameraPos", "numPointLights"
	};

	const std::vector<std::string> Material::CAMERA_UNIFORMS_PREFIX = {
			"globalLight.", "pointLights["
	};

	Material::Material() {
		_shader = rscrM().getShader("default");
		populateFromShader();
	}

	void Material::apply() const {
		if (!_shader) return;
		_shader->use();

		// Manda la luz global
		auto gl = sceneM().activeScene()->globalLight();

		_shader->setUniform("globalLight.direction", glm::normalize(gl->direction()));
		_shader->setUniform("globalLight.color", gl->color());
		_shader->setUniform("globalLight.intensity", gl->intensity());

		lightM().uploadToShader(_shader);


		// Sube todos los uniforms (excepto los de cámara que ya fueron enviados)
		for (auto& [name, prop] : _properties) {
			// No sobrescribir uniforms de cámara/luces que ya fueron enviados
			if (isUniform(name)) continue;

			std::visit([&](auto&& val) {
				using T = std::decay_t<decltype(val)>;

				if constexpr (std::is_same_v<T, int>)        _shader->setUniform(name, (int)val);
				else if constexpr (std::is_same_v<T, float>)      _shader->setUniform(name, (float)val);
				else if constexpr (std::is_same_v<T, bool>)       _shader->setUniform(name, (bool)val);
				else if constexpr (std::is_same_v<T, glm::vec2>)  _shader->setUniform(name, (glm::vec2)val);
				else if constexpr (std::is_same_v<T, glm::vec3>)  _shader->setUniform(name, (glm::vec3)val);
				else if constexpr (std::is_same_v<T, glm::vec4>)  _shader->setUniform(name, (glm::vec4)val);
				else if constexpr (std::is_same_v<T, glm::mat3>)  _shader->setUniform(name, (glm::mat3)val);
				else if constexpr (std::is_same_v<T, glm::mat4>)  _shader->setUniform(name, (glm::mat4)val);
				}, prop.value);
		}

		// Sube texturas por slots
		int slot = 0;
		for (auto& [name, tex] : _textures) {
			if (tex) {
				glActiveTexture(GL_TEXTURE0 + slot);
				tex->bind();
				_shader->setUniform(name, slot);
				++slot;
			}
		}
	}

	void Material::setShader(Shader* newShader) {
		_shader = newShader;
		_properties.clear();
		populateFromShader();
	}

	void Material::populateFromShader() {
		if (!_shader) return;

		for (auto& [name, type] : _shader->getActiveUniforms()) {
			if (isUniform(name)) continue;
			// Samplers van al mapa de texturas, no al de propiedades
			if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) {
				if (!_textures.count(name))
					_textures[name] = nullptr;
				continue;
			}

			// No pisar valores que ya existen
			if (_properties.count(name)) continue;

			// Crear valor por defecto según el tipo OpenGL
			switch (type) {
			case GL_FLOAT:
				if (name == "material.shininess") _properties[name] = MaterialProperty{ 32.0f };
				else _properties[name] = MaterialProperty{ 1.0f };
				break;
			case GL_INT:        _properties[name] = MaterialProperty{ 0 };               break;
			case GL_BOOL:       _properties[name] = MaterialProperty{ false };           break;
			case GL_FLOAT_VEC2: _properties[name] = MaterialProperty{ glm::vec2(1.0f) }; break;
			case GL_FLOAT_VEC3:
				if (name == "material.ambient")  _properties[name] = MaterialProperty{ glm::vec3(0.1f) };
				else if (name == "material.diffuse")  _properties[name] = MaterialProperty{ glm::vec3(0.8f) };
				else if (name == "material.specular") _properties[name] = MaterialProperty{ glm::vec3(0.5f) };
				else _properties[name] = MaterialProperty{ glm::vec3(1.0f) };
				break;
			case GL_FLOAT_VEC4: _properties[name] = MaterialProperty{ glm::vec4(1.0f) }; break;
			case GL_FLOAT_MAT3: _properties[name] = MaterialProperty{ glm::mat3(1.0f) }; break;
			case GL_FLOAT_MAT4: _properties[name] = MaterialProperty{ glm::mat4(1.0f) }; break;
			default: break;
			}
		}
	}

	void Material::serialize(JsonSerializer& s) const {
		s.write("type", (int)_shader->type());
		s.beginScope("uniforms");
		for (auto& [name, p] : _properties) {
			std::visit([&](auto&& val) {
				using T = std::decay_t<decltype(val)>;
				if constexpr (std::is_same_v<T, int>)         s.write(name, (int)val);
				else if constexpr (std::is_same_v<T, float>)  s.write(name, (float)val);
				else if constexpr (std::is_same_v<T, bool>)   s.write(name, (bool)val);
				else if constexpr (std::is_same_v<T, glm::vec2>) s.write(name, (glm::vec2)val);
				else if constexpr (std::is_same_v<T, glm::vec3>) s.write(name, (glm::vec3)val);
				else if constexpr (std::is_same_v<T, glm::vec4>) s.write(name, (glm::vec4)val);
				}, p.value);
		}
		s.endScope(); // cierra uniforms

		s.beginArray("textures");
		for (auto& [name, tex] : _textures) {
			if (!tex) continue;
			s.pushObjectToArray();
			s.write("uniformName", name);
			s.write("textureName", tex->name());
			s.endScope(); // cierra objeto textura
		}
		s.endScope(); // cierra array textures
	}

	void Material::deserialize(JsonSerializer& s) {
		_shader->setType(s.readInt("type"));
		s.beginScope("uniforms");
		int count = (int)s.getScopeSize(); // número de keys en el scope uniforms
		for (int i = 0; i < count; i++) {
			std::string key = s.getKey(i);
			JsonValue jval = s.getValue(i);

			std::visit([&](auto&& v) {
				using T = std::decay_t<decltype(v)>;
				if constexpr (std::is_constructible_v<UniformValue, T>)
					_properties[key] = MaterialProperty(UniformValue(v));
				}, jval);
		}
		s.endScope(); // cierra uniforms

		s.beginArray("textures");
		int texturesCount = (int)s.getArraySize();
		for (int i = 0; i < texturesCount; i++) {
			s.enterElement(i);
			auto uniformName = s.readString("uniformName");
			auto textureName = s.readString("textureName");
			_textures[uniformName] = rscrM().getTexture(textureName);
			s.endScope(); // cierra elemento i
		}
		s.endScope(); // cierra array textures
	}

	bool Material::isUniform(const std::string& name) const {
		if (Material::CAMERA_UNIFORMS.count(name)) return true;
		for (const auto& prefix : Material::CAMERA_UNIFORMS_PREFIX)
			if (name.starts_with(prefix)) return true;
		return false;
	}
}