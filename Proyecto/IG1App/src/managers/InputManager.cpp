#include "InputManager.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>

namespace cme {
	InputManager::InputManager() {
		StateChanger toMoving([]() {
			return glfwGetMouseButton(gla().window(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
		}, 
		CME_STATE_VIEWPORT_MOVING, 
		[]() {
			sceneM().activeScene()->getCamera()->firstMove();
			glfwSetInputMode(gla().window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		});

		StateChanger toNormal([]() {
			return glfwGetMouseButton(gla().window(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE;
		},
		CME_STATE_NORMAL,
		[]() {
			glfwSetInputMode(gla().window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		});

		addStateChanger(toMoving);
		addStateChanger(toNormal);

		float speed = 2.5f;
		std::vector<int> key = { GLFW_KEY_W };
		Shortcut cameraMoveW(key, [speed]() {
			Camera* cam = sceneM().activeScene()->getCamera();
			glm::vec3 cameraPos = cam->getPosition();
			glm::vec3 cameraFront = cam->getCameraFront();

			cameraPos += speed * cameraFront * gla().deltaTime();
			cam->setPosition(cameraPos);
		}, CME_STATE_VIEWPORT_MOVING);

		key = { GLFW_KEY_S };
		Shortcut cameraMoveS(key, [speed]() {
			Camera* cam = sceneM().activeScene()->getCamera();
			glm::vec3 cameraPos = cam->getPosition();
			glm::vec3 cameraFront = cam->getCameraFront();

			cameraPos -= speed * cameraFront * gla().deltaTime();
			cam->setPosition(cameraPos);
		}, CME_STATE_VIEWPORT_MOVING);

		key = { GLFW_KEY_A };
		Shortcut cameraMoveA(key, [speed]() {
			Camera* cam = sceneM().activeScene()->getCamera();
			glm::vec3 cameraPos = cam->getPosition();
			glm::vec3 cameraFront = cam->getCameraFront();
			glm::vec3 cameraUp = cam->getCameraUp();

			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * gla().deltaTime();
			cam->setPosition(cameraPos);
			}, CME_STATE_VIEWPORT_MOVING);

		key = { GLFW_KEY_D };
		Shortcut cameraMoveD(key, [speed]() {
			Camera* cam = sceneM().activeScene()->getCamera();
			glm::vec3 cameraPos = cam->getPosition();
			glm::vec3 cameraFront = cam->getCameraFront();
			glm::vec3 cameraUp = cam->getCameraUp();

			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * gla().deltaTime();
			cam->setPosition(cameraPos);
			}, CME_STATE_VIEWPORT_MOVING);

		addShortcut(cameraMoveW);
		addShortcut(cameraMoveS);
		addShortcut(cameraMoveA);
		addShortcut(cameraMoveD);
	}

	InputManager::~InputManager() {
		_shortcuts.clear();
		_stateChangers.clear();
	}

	bool Shortcut::isPressed(int currState) {
		bool pressedKeys = true;
		int i = 0;
		while (pressedKeys && i < keys.size()) {
			pressedKeys = glfwGetKey(gla().window(), keys[i]) == eventTrigger;
			i++;
		}

		return currState == state && pressedKeys;
	}

	void InputManager::proccessInput() {
		for (auto& sc : _stateChangers) {
			if (sc.condition()) {
				if (_currentState != sc.state)
					sc.call();

				_currentState = sc.state;
			}
		}

		for (auto& s : _shortcuts) {
			if (s.isPressed(_currentState)) {
				s.callback();
			}
		}
	}

	bool InputManager::init() {
		return true;
	}
}