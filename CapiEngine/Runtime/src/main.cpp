#include <GLApplication.h>
#include <iostream>
#include <utils/logger.h>
#include <filesystem>
#include <managers/SceneManager.h>
#include <managers/InputManager.h>

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/Scene.h>
#include <core/Camera.h>

using namespace cme;

void inputs();

int main(int argc, char* argv[]) {
	if (!cme::GLApplication::Init()) {
		LOG_ERROR("No se pudo inicializar GLApplication");
		return -1;
	}

	std::string path;
	if (argc > 1) {
		path = argv[1];
	}
	cme::sceneM().loadScene(path);

	inputs();

	cme::gla().run();
	cme::gla().Release();

    return 0;
}

void inputs() {
	// ----- MOVIMIENTO DEL VIEWPORT -----
	std::vector<int> key = { GLFW_KEY_W };
	cme::Shortcut cameraMoveW(key, []() {
		Camera* cam = sceneM().activeScene()->getCamera();
		glm::vec3 cameraPos = cam->getPosition();
		glm::vec3 cameraFront = cam->getCameraFront();

		cameraPos += cam->movementSpeed() * cameraFront * gla().deltaTime();
		cam->setPosition(cameraPos);
		}, CME_STATE_NORMAL);

	key = { GLFW_KEY_S };
	Shortcut cameraMoveS(key, []() {
		Camera* cam = sceneM().activeScene()->getCamera();
		glm::vec3 cameraPos = cam->getPosition();
		glm::vec3 cameraFront = cam->getCameraFront();

		cameraPos -= cam->movementSpeed() * cameraFront * gla().deltaTime();
		cam->setPosition(cameraPos);
		}, CME_STATE_NORMAL);

	key = { GLFW_KEY_A };
	Shortcut cameraMoveA(key, []() {
		Camera* cam = sceneM().activeScene()->getCamera();
		glm::vec3 cameraPos = cam->getPosition();
		glm::vec3 cameraFront = cam->getCameraFront();
		glm::vec3 cameraUp = cam->getCameraUp();

		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cam->movementSpeed() * gla().deltaTime();
		cam->setPosition(cameraPos);
		}, CME_STATE_NORMAL);

	key = { GLFW_KEY_D };
	Shortcut cameraMoveD(key, []() {
		Camera* cam = sceneM().activeScene()->getCamera();
		glm::vec3 cameraPos = cam->getPosition();
		glm::vec3 cameraFront = cam->getCameraFront();
		glm::vec3 cameraUp = cam->getCameraUp();

		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cam->movementSpeed() * gla().deltaTime();
		cam->setPosition(cameraPos);
		}, CME_STATE_NORMAL);

	key = { GLFW_KEY_LEFT_SHIFT };
	Shortcut fastMove(key, []() {
		Camera* cam = sceneM().activeScene()->getCamera();
		float movspeed = cam->movementSpeed();
		movspeed = cam->FAST_SPEED;
		cam->setMovementSpeed(movspeed);
		}, CME_STATE_NORMAL);

	key = { GLFW_KEY_LEFT_SHIFT };
	Shortcut normalMove(key, []() {
		Camera* cam = sceneM().activeScene()->getCamera();
		float movspeed = cam->movementSpeed();
		movspeed = cam->SLOW_SPEED;
		cam->setMovementSpeed(movspeed);
		}, CME_STATE_NORMAL, GLFW_RELEASE);

	inpM().addShortcut(cameraMoveW);
	inpM().addShortcut(cameraMoveS);
	inpM().addShortcut(cameraMoveA);
	inpM().addShortcut(cameraMoveD);
	inpM().addShortcut(fastMove);
	inpM().addShortcut(normalMove);
}