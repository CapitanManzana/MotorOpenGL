#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLApplication.h"
#include <utils/logger.h>

#include <managers/ResourceManager.h>
#include <managers/SceneManager.h>
#include <managers/InputManager.h>
#include <core/Scene.h>
#include <core/Camera.h>

#include <register/ComponentRegistry.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <component/Light.h>

namespace cme {
	GLApplication::~GLApplication() {
		if (ResourceManager::HasInstance()) {
			ResourceManager::Release();
		}

		if (SceneManager::HasInstance()) {
			SceneManager::Release();
		}

		if (Logger::HasInstance()) {
			Logger::Release();
		}

		if (InputManager::HasInstance()) {
			InputManager::Release();
		}

		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	bool GLApplication::init(std::string name) {
		// inicializar GLFW
		if (glfwInit() == GLFW_FALSE) {
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		_window = glfwCreateWindow(_width, _height, std::string("Capi Engine - " + name).c_str(), NULL, NULL);
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

		if (!loadManagers()) return false;

		return true;
	}

	bool GLApplication::loadManagers() {
		if (!Logger::Init()) {
			LOG_ERROR("Error al inicializar el Logger");
			return false;
		}

		if (!ResourceManager::Init()) {
			LOG_ERROR("Error al inicializar el Resource Manager");
			return false;
		}

		if (!SceneManager::Init()) {
			LOG_ERROR("Error al inicializar el Scene Manager");
			return false;
		}

		if (!InputManager::Init()) {
			LOG_ERROR("Error al inicializar el Scene Manager");
			return false;
		}

		glfwSetCursorPosCallback(_window, inpM().mouseCallback);

		// Registrar Componentes
		ComponentRegistry::registerComponent<Transform>("Transform");
		ComponentRegistry::registerComponent<MeshRenderer>("MeshRenderer");
		ComponentRegistry::registerComponent<Light>("Light");


		return true;
	}

	void GLApplication::run() {
		start();
		while (!glfwWindowShouldClose(_window))
		{
			update();
			render();
			swapAndTime();
		}
	}

	void GLApplication::update() {
		inpM().proccessInput();

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		sceneM().update();
	}

	void GLApplication::render() const {
		sceneM().render();
	}

	void GLApplication::start() {
		sceneM().start();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void GLApplication::swapAndTime() {
		glfwSwapBuffers(_window);
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;
	}
}
