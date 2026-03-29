#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cme {
	class Shader;
	class Mesh;

	/// @brief Clase que controla el punto de vista del la ventana, almacena la matriz de vista y la de proyeccion. Se puede configurar para que la camara renderice en formato perspectiva u ortografico.
	class Camera
	{
	private:
		/// @brief Matriz de vista
		glm::mat4 _view = glm::mat4(1.0f);
		/// @brief Matriz de pryección
		glm::mat4 _projection;

		glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 _cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 _cameraDirection = glm::normalize(_cameraPos - _cameraTarget);

		glm::vec3 _cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), _cameraDirection));

		glm::vec3 _cameraUp = glm::cross(_cameraDirection, _cameraRight);

		/// @brief Defide si se usa perspectiva (true) o si ortografico (false)
		bool _usePerspective = true;
		/// @brief El fiel of view de la camara
		float _fov = 45.0f;
		/// @brief Distancia a partir de donde se empiezan a ver las cosas
		float _nearDistance = 0.1f;
		/// @brief Distancia donde se dejan de ver las cosas
		float _farDistance = 100.0f;

		float _yaw = -132.0f;	// Respecto al eje y (desde arriba)
		float _pitch = -37.0f;	// Respecto al eje x (desde un lado)
		float _lastX = 0.0f;
		float _lastY = 0.0f;

		bool _firstMove = true; // Si es la primera vez que se mueve para que no se mueva muy brusco pone el offset del raton a 0

		float _movementSpeed;
	public:
		const float SLOW_SPEED = 2.5f;
		const float FAST_SPEED = 5.0f;

		Camera();

		/// @brief Manda a la GPU las matrices de proyeccion y vista
		/// @param m La mesh objetivo
		void uploadToGPU(Mesh* m);

		/// @brief La matriz de vista de la camara
		/// @return La matriz de vista
		glm::mat4 viewMatrix() { return _view; }

		/// @brief Se encarga de construir la matriz de proyeccion de dependiedo de los parametros
		void buildProjectionMat();

		/// @brief Establece la matriz de vista
		/// @param view La nueva matriz
		void setViewMat(glm::mat4 view) { _view = view; }

		/// @brief Modifica el yaw y el pitch para que la camara gire, se usa para el movimiento del raton
		/// @param xOffset El desplazamiento en el eje X del raton en ese frame
		/// @param yOffset El desplazamiento en el eje Y del raton en ese frame
		void setCameraLookAt(float xOffset, float yOffset);

		/// @brief Cuando se hace resize del viewport
		/// @param w El nuevo width
		/// @param h  El nuevo height
		void onResize(float w, float h);

		const glm::vec3& getPosition() const { return _cameraPos; }
		const glm::vec3& getCameraFront() const { return _cameraDirection; }
		const glm::vec3& getCameraUp() const { return _cameraUp; }

		float getYaw() const { return _yaw; }
		float getPitch() const { return _pitch; }

		void setPosition(const glm::vec3& pos) {
			_cameraPos = pos;
			_view = glm::lookAt(_cameraPos, _cameraPos + _cameraDirection, _cameraUp);
		}

		/// @brief Establece que que es la primera vez que se mueve la camara
		void firstMove() { _firstMove = true; }

		float movementSpeed() { return _movementSpeed; }
		void setMovementSpeed(float speed) { _movementSpeed = speed; }

		glm::mat4 getViewMat() { return _view; }
		glm::mat4 getProjectionMat() { return _projection; }

	private:
		/// @brief Manda a la gpu la matriz de proyeccion
		/// @param shader El shader objetivo a modificar
		void uploadProjectionToGPU(Shader* shader);

		/// @brief Manda a la gpu la matriz de vista
		/// @param shader El shader objetivo a modificar
		/// @param model La matriz de modelado del mesh
		void uploadViewToGPU(Shader* shader, glm::mat4 model);
	};
}

