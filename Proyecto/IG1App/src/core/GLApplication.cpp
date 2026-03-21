#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLApplication.h"
#include <utils/logger.h>

#include <managers/ResourceManager.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>

GLApplication::~GLApplication() {
	if (ResourceManager::HasInstance()) {
		ResourceManager::Release();
	}

	if (SceneManager::HasInstance()) {
		SceneManager::Release();
	}

	glfwTerminate();
}

bool GLApplication::init() {
	// inicializar GLFW
	if (glfwInit() == GLFW_FALSE) {
		return false;
	}

	_window = glfwCreateWindow(_width, _height, "LearnOpenGL", NULL, NULL);
	if (_window == NULL)
	{
		LOG_ERROR("Failed to create GLFW window");
		return false;
	}

	glfwMakeContextCurrent(_window);
	// Inicializad GLAD que carga los punteros a las funciones
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
		return false;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	if (!loadManagers()) return false;

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(_window, mouseCallback);

	return true;
}

bool GLApplication::loadManagers() {
	if (!ResourceManager::Init()) {
		LOG_ERROR("Error al inicializar el Resource Manager");
		return false;
	}

	if (!SceneManager::Init()) {
		LOG_ERROR("Error al inicializar el Scene Manager");
		return false;
	}

	return true;
}

void GLApplication::start() {
	sceneM().loadScenes();

	float angle = 0.0f;
	while (!glfwWindowShouldClose(_window))
	{
		processInput(_window, sceneM().activeScene()->getCamera());

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		sceneM().update();
		sceneM().render();

		glfwSwapBuffers(_window);
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;
	}
}

void GLApplication::processInput(GLFWwindow* window, Camera* cam)
{
	glm::vec3 cameraPos = cam->getPosition();
	glm::vec3 cameraFront = cam->getCameraFront();
	glm::vec3 cameraUp = cam->getCameraUp();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = 2.5f * _deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
		cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
		cameraSpeed;

	cam->setPosition(cameraPos);
}

void GLApplication::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	sceneM().activeScene()->getCamera()->setCameraLookAt(xpos, ypos);
}