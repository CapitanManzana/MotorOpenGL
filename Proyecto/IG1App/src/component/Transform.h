#pragma once
#include <ec/component.h>
#include <ec/ec.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform : public ec::Component{
private:
	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _scale = glm::vec3(1.0f);
	glm::vec3 _rotation = glm::vec3(0.0f);

public:
    __CMPID_DECL__(ec::comp::TRANSFORM)

    /// @brief Devuelve la matriz de modelado de la entidad
    /// @return Un glm::mat4
    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        // Escala
        model = glm::scale(model, _scale);
        // Rotación
        model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
        // Traslacion
        model = glm::translate(model, _position);
        return model;
    }

    // --- GETTERS ---
    const glm::vec3& getPosition() const { return _position; }
    const glm::vec3& getScale() const { return _scale; }
    const glm::vec3& getRotation() const { return _rotation; }

    // --- SETTERS ---
    void setPosition(const glm::vec3& pos) { _position = pos; }
    void setScale(const glm::vec3& scale) { _scale = scale; }
    void setRotation(const glm::vec3& rot) { _rotation = rot; }
};