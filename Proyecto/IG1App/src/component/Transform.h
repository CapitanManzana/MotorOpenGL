#pragma once
#include <ec/component.h>
#include <ec/ec.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/serialize/Serializable.h>

class Transform : public ec::Component, public capiEngine::Serializable {
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

    virtual void serialize(capiEngine::JsonSerializer& s) const override;
    virtual void deserialize(capiEngine::JsonSerializer& s) override;
};