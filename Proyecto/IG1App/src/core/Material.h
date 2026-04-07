#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace cme {
    class Shader;
    class Texture;
    enum class AlbedoMode { COLOR, TEXTURE };

    class Material {
    public:
        Material();

        Shader* shader = nullptr;
        AlbedoMode  albedoMode = AlbedoMode::COLOR;

        // Phong
        glm::vec3 ambient = glm::vec3(0.1f);
        glm::vec3 diffuse = glm::vec3(0.8f);
        glm::vec3 specular = glm::vec3(1.0f);
        float     shininess = 32.0f;

        // Albedo (color o textura)
        glm::vec4   albedoColor = glm::vec4(1.0f);
        Texture*    albedoTex = nullptr;
        std::string albedoTexName;

        void apply() const;
        void drawOnInspector();
    };
}
