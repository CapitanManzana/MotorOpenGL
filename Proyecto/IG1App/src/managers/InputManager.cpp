#include "InputManager.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <utils/FileExplorer.h>
#include <utils/Raycast.h>

namespace cme {
	InputManager::InputManager() {
		StateChanger toMoving([this]() {
			return this->_isViewportHovered && glfwGetMouseButton(gla().window(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
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

		MouseEvent selectObjInViewPortr([this]() {
			return this->_isViewportHovered && glfwGetMouseButton(gla().window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		}, []() {
			Raycast ray;
			auto ent = ray.castRay();
			if (auto lockedEnt = ent.lock()) {
				LOG_INFO(std::format("Se ha clicado en el objeto con nombre: {}", lockedEnt->name()));
			}
		});

		addMouseEvent(selectObjInViewPortr);

		addStateChanger(toMoving);
		addStateChanger(toNormal);

		createShortcuts();
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

		for (auto& m : _mouseEvents) {
			if (m.condition()) {
				m.call();
			}
		}
	}

	bool InputManager::init() {
		return true;
	}

	void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
		if (inpM()._currentState == CME_STATE_VIEWPORT_MOVING) {
			sceneM().activeScene()->getCamera()->setCameraLookAt(xpos, ypos);
			inpM()._mouseX = static_cast<float>(xpos);
			inpM()._mouseY = static_cast<float>(ypos);
		}
	}

	void InputManager::createShortcuts() {
		// ----- ATAJOS VENTANA MOTOR -----
		std::vector<int> saveFileKeys = { GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S };
		Shortcut saveFile(saveFileKeys, []() {
			FileExplorer f;
			std::string path = f.fileDialog(FileDialogMode::Save);
			if (path != "") sceneM().saveActiveScene(path);
		}, CME_STATE_NORMAL);
		addShortcut(saveFile);

		std::vector<int> loadFileKeys = { GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L };
		Shortcut loadFile(loadFileKeys, []() {
			FileExplorer f;
			std::string path = f.fileDialog(FileDialogMode::Open);
			if (path != "") sceneM().loadScene(path);
			}, CME_STATE_NORMAL);
		addShortcut(loadFile);

		// ----- MOVIMIENTO DEL VIEWPORT -----
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
}