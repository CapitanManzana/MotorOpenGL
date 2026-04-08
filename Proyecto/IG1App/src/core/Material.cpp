#include "Material.h"
#include "Shader.h"
#include <glad/glad.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <managers/ResourceManager.h>
#include <core/Texture.h>
#include <unordered_set>

namespace cme {
    Material::Material() {
        _shader = rscrM().getShader("default");
        populateFromShader();
    }

    void Material::apply() const {
        if (!_shader) return;
        _shader->use();

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
            "cameraPos", "lightColor", "lightPos",
            "hasLight", "ambientStrength"  
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
            case GL_FLOAT:      _properties[name] = MaterialProperty{ 1.0f };            break;
            case GL_INT:        _properties[name] = MaterialProperty{ 0 };               break;
            case GL_BOOL:       _properties[name] = MaterialProperty{ false };           break;
            case GL_FLOAT_VEC2: _properties[name] = MaterialProperty{ glm::vec2(1.0f) }; break;
            case GL_FLOAT_VEC3: _properties[name] = MaterialProperty{ glm::vec3(1.0f) }; break;
            case GL_FLOAT_VEC4: _properties[name] = MaterialProperty{ glm::vec4(1.0f) }; break;
            /*case GL_FLOAT_MAT3: _properties[name] = MaterialProperty{ glm::mat3(1.0f) }; break;
            case GL_FLOAT_MAT4: _properties[name] = MaterialProperty{ glm::mat4(1.0f) }; break;*/
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
        for (auto& [name, tex] : _textures) {
            // Preview
            if (tex)
                ImGui::Image((ImTextureID)(intptr_t)tex->id(), ImVec2(48, 48));
            else
                ImGui::Dummy(ImVec2(48, 48));

            ImGui::SameLine();

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
        }
    }
}