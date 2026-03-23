#include "Transform.h"
#include <core/serialize/JsonSerializer.h>

glm::mat4 Transform::getModelMatrix() const {
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

void Transform::serialize(capiEngine::JsonSerializer& s) const {
    s.write("position", _position);
    s.write("scale", _scale);
    s.write("rotation", _rotation);
}

void Transform::deserialize(capiEngine::JsonSerializer& s) {
    _position = s.readVec3("position");
    _scale = s.readVec3("scale");
    _rotation = s.readVec3("rotation");
}