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

		std::vector<int> key = { GLFW_KEY_W };
		Shortcut cameraMoveW(key, []() {
			Camera* cam = sceneM().activeScene()->getCamera();
			glm::vec3 cameraPos = cam->getPosition();
			glm::vec3 cameraFront = cam->getCameraFront();

			cameraPos += cam->movementSpeed() * cameraFront * gla().deltaTime();
			cam->setPosition(cameraPos);
		}, CME_STATE_VIEWPORT_MOVING);

		key = { GLFW_KEY_S };
		Shortcut cameraMoveS(key, []() {
			Camera* cam = sceneM().activeScene()->getCamera();
			glm::vec3 cameraPos = cam->getPosition();
			glm::vec3 cameraFront = cam->getCameraFront();

			cameraPos -= cam->movementSpeed() * cameraFront * gla().deltaTime();
			cam->setPosition(cameraPos);
		}, CME_STATE_VIEWPORT_MOVING);

		key = { GLFW_KEY_A };
		Shortcut cameraMoveA(key, []() {
			Camera* cam = sceneM().activeScene()->getCamera();
			glm::vec3 cameraPos = cam->getPosition();
			glm::vec3 cameraFront = cam->getCameraFront();
			glm::vec3 cameraUp = cam->getCameraUp();

			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cam->movementSpeed() * gla().deltaTime();
			cam->setPosition(cameraPos);
			}, CME_STATE_VIEWPORT_MOVING);

		key = { GLFW_KEY_D };
		Shortcut cameraMoveD(key, []() {
			Camera* cam = sceneM().activeScene()->getCamera();
			glm::vec3 cameraPos = cam->getPosition();
			glm::vec3 cameraFront = cam->getCameraFront();
			glm::vec3 cameraUp = cam->getCameraUp();

			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cam->movementSpeed() * gla().deltaTime();
			cam->setPosition(cameraPos);
			}, CME_STATE_VIEWPORT_MOVING);

		key = { GLFW_KEY_LEFT_SHIFT };
		Shortcut fastMove(key, []() {
			Camera* cam = sceneM().activeScene()->getCamera();
			float movspeed = cam->movementSpeed();
			movspeed = cam->FAST_SPEED;
			cam->setMovementSpeed(movspeed);
			}, CME_STATE_VIEWPORT_MOVING);

		key = { GLFW_KEY_LEFT_SHIFT };
		Shortcut normalMove(key, []() {
			Camera* cam = sceneM().activeScene()->getCamera();
			float movspeed = cam->movementSpeed();
			movspeed = cam->SLOW_SPEED;
			cam->setMovementSpeed(movspeed);
			}, CME_STATE_VIEWPORT_MOVING, GLFW_RELEASE);

		addShortcut(cameraMoveW);
		addShortcut(cameraMoveS);
		addShortcut(cameraMoveA);
		addShortcut(cameraMoveD);
		addShortcut(fastMove);
		addShortcut(normalMove);
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

	void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
		if (inpM()._currentState == CME_STATE_VIEWPORT_MOVING) {
			sceneM().activeScene()->getCamera()->setCameraLookAt(xpos, ypos);
		}
	}
}