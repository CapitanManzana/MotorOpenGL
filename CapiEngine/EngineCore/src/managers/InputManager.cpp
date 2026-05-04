#include <managers/InputManager.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <utils/Raycast.h>


namespace cme {
	InputManager::InputManager() {
		
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
			if (m.condition() && !m.pressed) {
				m.call();
				m.pressed = true;
			}
			else if (glfwGetMouseButton(gla().window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && m.pressed) {
				m.pressed = false;
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

	bool InputManager::isKeyPressed(int key) const {
		return glfwGetKey(gla().window(), key) == GLFW_PRESS;
	}
	bool InputManager::isKeyReleased(int key) const {
		return glfwGetKey(gla().window(), key) == GLFW_RELEASE;
	}
	bool InputManager::isMouseButtonPressed(int button) const {
		return glfwGetMouseButton(gla().window(), button) == GLFW_PRESS;
	}
}