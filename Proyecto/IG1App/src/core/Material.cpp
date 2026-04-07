#include "Material.h"
#include "Shader.h"
#include <glad/glad.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <managers/ResourceManager.h>
#include <core/Texture.h>

namespace cme {
    Material::Material() {
        shader = rscrM().getShader("default");
    }

    void Material::apply() const {
        if (!shader) return;
        shader->use();

        // Componentes Phong
        shader->setUniform("material.ambient", ambient);
        shader->setUniform("material.diffuse", diffuse);
        shader->setUniform("material.specular", specular);
        shader->setUniform("material.shininess", shininess);

        // Albedo
        bool useTexture = (albedoMode == AlbedoMode::TEXTURE && albedoTex != 0);
        shader->setUniform("useAlbedoTex", useTexture);

        if (useTexture) {
            glActiveTexture(GL_TEXTURE0);
            albedoTex->bind();
            shader->setUniform("albedoMap", 0);
        }
        else {
            shader->setUniform("albedo", albedoColor);
        }
    }

    void Material::drawOnInspector() {
        if (!ImGui::CollapsingHeader("Material (Phong)")) return;

        // --- Albedo ---
        const char* modos[] = { "Color", "Textura" };
        int modoActual = (int)albedoMode;
        if (ImGui::Combo("Albedo##modo", &modoActual, modos, 2))
            albedoMode = (AlbedoMode)modoActual;

        ImGui::Indent();
        if (albedoMode == AlbedoMode::COLOR) {
            ImGui::ColorEdit4("Color##albedo", glm::value_ptr(albedoColor));
        }
        else {
            if (ImGui::BeginCombo("Textura##albedo", albedoTexName.c_str())) {
                for (auto& nombre : rscrM().getAllTextureNames()) {
                    bool sel = (albedoTexName == nombre);
                    if (ImGui::Selectable(nombre.c_str(), sel)) {
                        albedoTexName = nombre;
                        albedoTex = rscrM().getTexture(nombre);
                    }
                    if (sel) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if (albedoTex != 0)
                ImGui::Image((ImTextureID)(intptr_t)albedoTex->id(), ImVec2(64, 64));
        }
        ImGui::Unindent();

        ImGui::Separator();

        // --- Componentes Phong ---
        ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
        ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f, "%.0f",
            ImGuiSliderFlags_Logarithmic); // log porque 32 y 256 se sienten muy distintos
    }
}