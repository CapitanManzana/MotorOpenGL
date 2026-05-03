#pragma once
#include <ec/component.h>
#include <ec/ec.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cme {
    /// @brief Componente que controla la posici�n, rotacion y escala en el mundo de la entidad
    class Transform : public ec::Component {
    private:
        glm::vec3 _position = glm::vec3(0.0f);
        glm::vec3 _scale = glm::vec3(1.0f);
        glm::vec3 _rotation = glm::vec3(0.0f);

    public:
        __CMPID_DECL__(ec::comp::TRANSFORM)

        /// @brief Devuelve la matriz de modelado de la entidad
        /// @return Un glm::mat4
        glm::mat4 getModelMatrix() const;

        // --- GETTERS ---
        const glm::vec3& getPosition() const { return _position; }
        glm::vec3& getPosition() { return _position; }
        const glm::vec3& getScale() const { return _scale; }
        glm::vec3& getScale() { return _scale; }
        const glm::vec3& getRotation() const { return _rotation; }
        glm::vec3& getRotation() { return _rotation; }

        // --- SETTERS ---
        void setPosition(const glm::vec3& pos) { _position = pos; }
        void setScale(const glm::vec3& scale) { _scale = scale; }
        void setRotation(const glm::vec3& rot) { _rotation = rot; }

        void serialize(JsonSerializer& s) const override;
        void deserialize(JsonSerializer& s) override;

        std::string serializeID() const override { return "Transform"; }
    };
}