#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <variant>
#include <core/serialize/Serializable.h>
#include <unordered_set>
#include <vector>

namespace cme {
    class Shader;
    class Texture;
    enum class AlbedoMode { COLOR, TEXTURE };

    using UniformValue = std::variant<
        int,
        float,
        bool,
        glm::vec2,
        glm::vec3,
        glm::vec4,
        glm::mat3,
        glm::mat4
    >;

    struct MaterialProperty {
        UniformValue value;

        MaterialProperty() : value(0.0f) {}
        MaterialProperty(const UniformValue& v) : value(v) {}
        MaterialProperty(UniformValue&& v) : value(std::move(v)) {}
    };

    class Material : public Serializable {
    private:
        Shader* _shader = nullptr;
        std::unordered_map<std::string, MaterialProperty> _properties;
        std::unordered_map<std::string, Texture*>         _textures;

        static const std::unordered_set<std::string> CAMERA_UNIFORMS;
        static const std::vector<std::string> CAMERA_UNIFORMS_PREFIX;

    public:
        Material();

        // --- Shader ---
        void setShader(Shader* shader);
        Shader* getShader() const { return _shader; }

        void setInt(const std::string& name, int v) { _properties[name] = MaterialProperty(UniformValue(v)); }
        void setFloat(const std::string& name, float v) { _properties[name] = MaterialProperty(UniformValue(v)); }
        void setBool(const std::string& name, bool v) { _properties[name] = MaterialProperty(UniformValue(v)); }
        void setVec2(const std::string& name, glm::vec2 v) { _properties[name] = MaterialProperty(UniformValue(v)); }
        void setVec3(const std::string& name, glm::vec3 v) { _properties[name] = MaterialProperty(UniformValue(v)); }
        void setVec4(const std::string& name, glm::vec4 v) { _properties[name] = MaterialProperty(UniformValue(v)); }

        // --- Texturas (aparte, tienen slots) ---
        void setTexture(const std::string& name, Texture* tex) { _textures[name] = tex; }

        // --- Sube todo al shader ---
        void apply() const;

        void drawOnInspector();

        void serialize(JsonSerializer& s) const override;
        void deserialize(JsonSerializer& s) override;

    private:
        // Rellena _properties con los uniforms por defecto del shader
        void populateFromShader();
        bool isUniform(const std::string& name) const;
    };
}
