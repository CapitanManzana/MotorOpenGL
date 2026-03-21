#pragma once
#include <utils/Singleton.h>

struct GLFWwindow;
class Camera;
class Scene;

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
	float _width = 800;
	float _height = 600;

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
	/// @brief Devuelve la diferencia de tiempo entre frames
	/// @return El delta time
	float deltaTime() { return _deltaTime; }
	
private:
	GLApplication() = default;

	bool init();
	/// @brief Carga las instancias de los managers
	/// @return True si se carga correctamente
	bool loadManagers();
	/// @brief Procesa los inputs durante la ejecución
	/// @param window La ventana de los inputs
	void processInput(GLFWwindow* window, Camera* cam);
	/// @brief Procesa el input del raton, se llama omo callback
	/// @param window LA ventana
	/// @param xpos La posicion del raton en el eje X
	/// @param ypos La posicion del raton en el eje Y
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};

/// @brief Obtiene la instancia de la clase
/// @return Referencia de la clase GLApplication activa
inline GLApplication& gla() {
	return *GLApplication::Instance();
}

