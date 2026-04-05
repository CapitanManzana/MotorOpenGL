#include "Camera.h"
#include <core/GLApplication.h>
#include <core/Shader.h>
#include <utils/logger.h>
#include <managers/ResourceManager.h>
#include <core/Mesh.h>
#include <component/Light.h>

namespace cme {
	Camera::Camera() {
		_lastX = gla().width() / 2;
		_lastY = gla().height() / 2;

		setPosition(glm::vec3(4, 4, 4));
		glm::vec3 direction;
		direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		direction.y = sin(glm::radians(_pitch));
		direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_cameraDirection = glm::normalize(direction);

		buildProjectionMat();
		_view = glm::lookAt(_cameraPos, _cameraPos + _cameraDirection, _cameraUp);

		_movementSpeed = SLOW_SPEED;
	}

	void Camera::buildProjectionMat() {
		if (_usePerspective) {
			_projection = glm::perspective(_fov, gla().width() / gla().height(), _nearDistance, _farDistance);
		}
		else {
			_projection = glm::ortho(0.0f, gla().width(), 0.0f, gla().height(), _nearDistance, _farDistance);
		}
	}

	void Camera::uploadToGPU(Mesh* m, ec::entity_t ent) {
		Shader* s = m->shader();
		uploadProjectionToGPU(s);
		uploadViewToGPU(s, m->modelMatrix(), ent);
	}

	void Camera::uploadProjectionToGPU(Shader* shader) {
		if (!shader) {
			LOG_WARN("El shader para hacer upload de la matriz de proyeccion es nulo");
			return;
		}

		shader->use();
		shader->setUniform("projection", _projection);
	}

	void Camera::uploadViewToGPU(Shader* shader, glm::mat4 model, ec::entity_t ent) {
		if (!shader) {
			LOG_WARN("El shader para hacer upload de la matriz de vista es nulo");
			return;
		}

		glm::mat4 modelView = _view * model;
		shader->use();

		shader->setUniform("modelView", modelView);
		shader->setUniform("cameraPos", _cameraPos);
		shader->setUniform("model", model);

		if (ent->hasComponent<Light>()) return;

		auto& lights = Light::getAllLights();
		Light* l = nullptr;
		if (!lights.empty()) {
			l = lights[0];
			shader->setUniform("lightColor", l->color());
			shader->setUniform("lightPos", l->getPosition());
			shader->setUniform("ambientStrength", l->ambientStrength());
		}
		else {
			shader->setUniform("lightColor", glm::vec3(1.f, 1.f, 1.f));
			shader->setUniform("ambientStrenght", 1.0f);
		}
	}

	void Camera::setCameraLookAt(float xpos, float ypos) {
		if (_firstMove) {
			_lastX = xpos;
			_lastY = ypos;
			_firstMove = false;
		}

		float xoffset = xpos - _lastX;
		float yoffset = _lastY - ypos; // reversed: y ranges bottom to top
		_lastX = xpos;
		_lastY = ypos;
		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		_yaw += xoffset;
		_pitch += yoffset;

		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		direction.y = sin(glm::radians(_pitch));
		direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_cameraDirection = glm::normalize(direction);
		_view = glm::lookAt(_cameraPos, _cameraPos + _cameraDirection, _cameraUp);
	}

	void Camera::onResize(float w, float h) {
		_projection = glm::perspective(glm::radians(_fov), w / h, _nearDistance, _farDistance);
	}
}