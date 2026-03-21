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
#include <core/ui/UIManager.h>

GLApplication::~GLApplication() {
	if (ResourceManager::HasInstance()) {
		ResourceManager::Release();
	}

	if (SceneManager::HasInstance()) {
		SceneManager::Release();
	}
	delete _interface;
	glfwTerminate();
}

bool GLApplication::init() {
	// inicializar GLFW
	if (glfwInit() == GLFW_FALSE) {
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	_window = glfwCreateWindow(_width, _height, "Manza Engine", NULL, NULL);
	if (_window == NULL)
	{
		LOG_ERROR("Failed to create GLFW window");
		return false;
	}
	int w = 0, h = 0;
	glfwGetWindowSize(_window, &w, &h);
	_width = w;
	_height = h;

	glfwMakeContextCurrent(_window);
	// Inicializad GLAD que carga los punteros a las funciones
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
		return false;
	}

	// Init viewPort
	glViewport(0, 0, _width, _height);
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		gla()._width = width;
		gla()._height = height;

		sceneM().activeScene()->getCamera()->buildProjectionMat();
		});
	glfwSetCursorPosCallback(_window, mouseCallback);

	if (!loadManagers()) return false;

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

	_interface = new capiEngine::ui::UIManager();
	if (!_interface->initCoreUI(_window)) {
		LOG_ERROR("Error al inicializar la interfaz del motor");
		return false;
	}

	return true;
}

void GLApplication::start() {
	sceneM().loadScenes();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(_window))
	{
		processInput(_window, sceneM().activeScene()->getCamera());

		glClearColor(0.3f, 0.3f, 0.31f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		sceneM().update();
		sceneM().render();
		_interface->render();

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

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (_glaState == GLA_MODE_NORMAL) {
			cam->firstMove();
		}

		_glaState = GLA_MODE_MOVING;

		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float cameraSpeed = 0.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cameraSpeed = 5.0f * _deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			cameraSpeed = 2.5f * _deltaTime;

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
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		_glaState = GLA_MODE_NORMAL;
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	cam->setPosition(cameraPos);
}

void GLApplication::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (gla()._glaState == GLA_MODE_MOVING) {
		sceneM().activeScene()->getCamera()->setCameraLookAt(xpos, ypos);
	}
}
