#pragma once
#include <utils/Singleton.h>

struct GLFWwindow;

/**
 * @namespace cme
 * @brief Contiene todas las clases principales y sistemas del Core Engine.
 * * En este namespace se agrupan utilidades matemáticas, gestores de ventana,
 * el bucle principal y los componentes del motor.
 */
namespace cme {
	class Camera;
	class Scene;
	namespace ui {
		class UIManager;
	}

	/**
	 * @brief Clase principal que maneja el ciclo de vida de la aplicación.
	 * * Utiliza el patrón Singleton para asegurar que solo exista una instancia
	 * de la ventana de GLFW en todo el proyecto OpenGL.
	 */
	class GLApplication : public Singleton<GLApplication>
	{
		friend class Singleton<GLApplication>;
	private:
		GLFWwindow* _window = nullptr;		// Puntero a la ventana de openGL
		ui::UIManager* _interface = nullptr;		// Puntero a la interfaz del motor
		float _width = 800;
		float _height = 600;

		float _viewportWidth = 800;
		float _viewportHeight = 600;

		float _deltaTime = 0.0f;
		float _lastFrame = 0.0f;

	public:
		virtual ~GLApplication();

		// cannot copy/move
		GLApplication(GLApplication&) = delete;
		GLApplication(GLApplication&&) = delete;
		GLApplication& operator=(GLApplication&) = delete;
		GLApplication& operator=(GLApplication&&) = delete;

		/// @brief Inicializa el bucle de la applicación
		void start();

		/// @brief El ancho de la pantalla
		/// @return El ancho de la pantalla
		float width() { return _width; }
		/// @brief El alto de la pantalla
		/// @return El alto de la pantalla
		float height() { return _height; }

		/// @brief El ancho del viewport
		/// @return El ancho del viewport
		float viewportWidth() { return _viewportWidth; }
		/// @brief El alto del viewport
		/// @return El alto del viewport
		float viewportHeight() { return _viewportHeight; }

		void setViewportWidth(float vw) { _viewportWidth = vw; }
		void setViewportHeight(float vh) { _viewportHeight = vh; }

		/// @brief Devuelve la diferencia de tiempo entre frames
		/// @return El delta time
		float deltaTime() { return _deltaTime; }

		GLFWwindow* window() { return _window; }

		ui::UIManager* getUIManager() { return _interface; }
	private:
		GLApplication() = default;

		bool init();
		/// @brief Carga las instancias de los managers
		/// @return True si se carga correctamente
		bool loadManagers();
	};

	/// @brief Obtiene la instancia de la clase
	/// @return Referencia de la clase GLApplication activa
	inline GLApplication& gla() {
		return *GLApplication::Instance();
	}
}

