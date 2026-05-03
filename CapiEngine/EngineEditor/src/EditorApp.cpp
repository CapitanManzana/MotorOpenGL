#include "EditorApp.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/Scene.h>
#include <core/Camera.h>
#include <utils/Raycast.h>
#include <utils/FileExplorer.h>

#include <managers/UIManager.h>
#include <windows/InspectorWindow.h>

namespace cme::editor {
	bool EditorApp::init() {
		if (!GLApplication::Init()) {
			LOG_ERROR("Error al inicializar el GLApplication");
			return false;
		}

		_ui = std::make_shared<UIManager>();
		if (!_ui->initCoreUI(gla().window())) {
			LOG_ERROR("Error al inicializar la interfaz del motor");
			return false;
		}

		// Configuramos eventos de la UI
		_ui->setCreateCubeCallback([]() {
			sceneM().activeScene()->addCubeToScene();
			});

		return true;
	}

	void EditorApp::run() {
		gla().start();
		_ui->start();

		createShortcuts();
		stateChangers();

		while (!glfwWindowShouldClose(gla().window())) {
			gla().update();

			_ui->bind();
			sceneM().render();
			_ui->unbind();

			_ui->render();

			gla().swapAndTime();
		}
	}

	void EditorApp::createShortcuts() {
		// ----- ATAJOS VENTANA MOTOR -----
		std::vector<int> saveFileKeys = { GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S };
		Shortcut saveFile(saveFileKeys, []() {
			FileExplorer f;
			std::string path = f.fileDialog(FileDialogMode::Save);
			if (path != "") sceneM().saveActiveScene(path);
			}, CME_STATE_NORMAL);
		inpM().addShortcut(saveFile);

		std::vector<int> loadFileKeys = { GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L };
		Shortcut loadFile(loadFileKeys, []() {
			FileExplorer f;
			std::string path = f.fileDialog(FileDialogMode::Open);
			if (path != "") sceneM().loadScene(path);
			}, CME_STATE_NORMAL);
		inpM().addShortcut(loadFile);

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

		inpM().addShortcut(cameraMoveW);
		inpM().addShortcut(cameraMoveS);
		inpM().addShortcut(cameraMoveA);
		inpM().addShortcut(cameraMoveD);
		inpM().addShortcut(fastMove);
		inpM().addShortcut(normalMove);
	}

	void EditorApp::stateChangers() {
		StateChanger toMoving([]() {
			return inpM().isViewportHovered() && glfwGetMouseButton(gla().window(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
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

		MouseEvent selectObjInViewPortr([]() {
			return inpM().isViewportHovered() && glfwGetMouseButton(gla().window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
			}, [this]() {
				Raycast ray;
				auto ent = ray.castRay();
				auto win = _ui->getWindow(INSPECTOR);
				if (auto lockedWin = win.lock()) {
					InspectorWindow& inspector = static_cast<InspectorWindow&>(*lockedWin);
					inspector.changeDisplayEntity(ent);
					LOG_INFO("Cambiando entidad del inspector");
				}
				}, GLFW_MOUSE_BUTTON_LEFT);

			inpM().addMouseEvent(selectObjInViewPortr);

			inpM().addStateChanger(toMoving);
			inpM().addStateChanger(toNormal);
	}

}