#include "component/Transform.h"
#include <serialize/JsonSerializer.h>

namespace cme {
	glm::mat4 Transform::getModelMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);
		// Primero trasladar
		model = glm::translate(model, _position);
		// Luego rotar sobre su propio eje
		model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
		// Por �ltimo escalar
		model = glm::scale(model, _scale);
		return model;
	}

	void Transform::serialize(JsonSerializer& s) const {
		s.write("position", _position);
		s.write("scale", _scale);
		s.write("rotation", _rotation);
	}

	void Transform::deserialize(JsonSerializer& s) {
		_position = s.readVec3("position");
		_scale = s.readVec3("scale");
		_rotation = s.readVec3("rotation");
	}
}