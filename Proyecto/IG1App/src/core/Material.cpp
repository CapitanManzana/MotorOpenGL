#include "Material.h"
#include "Shader.h"
#include <glad/glad.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <managers/ResourceManager.h>
#include <core/Texture.h>
#include <unordered_set>

#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/lighting/GlobalLight.h>

#include <core/serialize/JsonSerializer.h>

namespace cme {
	Material::Material() {
		_shader = rscrM().getShader("default");
		populateFromShader();
	}

	void Material::apply() const {
		if (!_shader) return;
		_shader->use();

		// Manda la luz global
		if (_shader->type() == ShaderType::LIT_PHONG) {
			auto gl = sceneM().activeScene()->globalLight();

			_shader->setUniform("globalLight.direction", glm::normalize(gl->direction()));
			_shader->setUniform("globalLight.color", gl->color());
			_shader->setUniform("globalLight.intensity", gl->intensity());
		}

		// Sube todos los uniforms
		for (auto& [name, prop] : _properties) {
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

		static const std::unordered_set<std::string> cameraUniforms = {
			"projection", "modelView", "model", "normalMatrix",
			"cameraPos", "lightColor", "lightPos", "hasLight", "ambientStrength",
			"globalLight.direction", "globalLight.color", "globalLight.intensity"
		};

		for (auto& [name, type] : _shader->getActiveUniforms()) {
			if (cameraUniforms.count(name)) continue;
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

	void Material::drawOnInspector() {
		if (!ImGui::CollapsingHeader("Material")) return;

		// Combo de shaders
		if (ImGui::BeginCombo("Shader", _shader ? _shader->getName().c_str() : "None")) {
			for (auto& name : rscrM().getAllShaderNames()) {
				if (ImGui::Selectable(name.c_str())) {
					setShader(rscrM().getShader(name));
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();

		// Propiedades dinámicas según el tipo
		for (auto& [name, prop] : _properties) {
			std::visit([&](auto&& val) {
				using T = std::decay_t<decltype(val)>;

				if constexpr (std::is_same_v<T, float>)
					ImGui::SliderFloat(name.c_str(), &std::get<float>(prop.value), 0.f, 256.f);
				else if constexpr (std::is_same_v<T, glm::vec3>)
					ImGui::ColorEdit3(name.c_str(), glm::value_ptr(std::get<glm::vec3>(prop.value)));
				else if constexpr (std::is_same_v<T, glm::vec4>)
					ImGui::ColorEdit4(name.c_str(), glm::value_ptr(std::get<glm::vec4>(prop.value)));
				else if constexpr (std::is_same_v<T, bool>)
					ImGui::Checkbox(name.c_str(), &std::get<bool>(prop.value));
				}, prop.value);
		}

		ImGui::Separator();
		ImGui::Text("Texturas");
		ImGui::Dummy(ImVec2(0, 10));
		for (auto& [name, tex] : _textures) {
			// Combo de texturas disponibles
			std::string label = tex ? tex->name() : "None";
			if (ImGui::BeginCombo(name.c_str(), label.c_str())) {
				// Opción para quitar textura
				if (ImGui::Selectable("None"))
					tex = nullptr;

				for (auto& texName : rscrM().getAllTextureNames()) {
					bool sel = (tex && tex->name() == texName);
					if (ImGui::Selectable(texName.c_str(), sel))
						tex = rscrM().getTexture(texName);
					if (sel) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();

			// Preview
			if (tex)
				ImGui::Image((ImTextureID)(intptr_t)tex->id(), ImVec2(48, 48));
			else
				ImGui::Dummy(ImVec2(48, 48));
		}
	}

	void Material::serialize(JsonSerializer& s) const {
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
}