#pragma once
#include <utils/Singleton.h>
#include <vector>
#include <functional>

#define CME_STATE_NORMAL 0
#define CME_STATE_VIEWPORT_MOVING 1

typedef std::function<void()> shortcutCall;
struct GLFWwindow;

namespace cme {
	/// @brief Estructurra que maneja los atajos de teclado
	struct Shortcut {
		std::vector<int> keys;					// Vector de teclas a pulsar
		int eventTrigger = 1;					// Cuando se ejecuta el shortcut, al pulsar, soltar... (Default 1 = GLFW_PRESS)
		int state = CME_STATE_NORMAL;			// El estado de cuando se tiene que ejecutar (Default CME_STATE_NORMAL)
		shortcutCall callback;					// La callback para cuando se pulsa

		/// @brief Crea un atajo de teclado para el motor
		/// @param k Las teclas a pulsar
		/// @param call La callback a ejecutar tras pulsar las teclas
		/// @param state El estado en el que se ejecuta
		/// @param eventTrgg El trigger del evento, cuando se realiza la accion, al soltar, pulsar, mantener...
		Shortcut(std::vector<int> k, shortcutCall call, int state = CME_STATE_NORMAL, int eventTrgg = 1) :
			keys(k), callback(call), state(state), eventTrigger(eventTrgg) { }

		/// @brief Si se cumplen las condiciones para ejecutar el atajo
		/// @param currState El estado actual
		/// @return True si se estan pulsando las tenclas y el estado es el correcto
		bool isPressed(int currState);
	};

	/// @brief Estructura que maneja los eventos del raton
	struct  MouseEvent {
		std::function<bool()> condition;			// Condicion a ocurrir
		std::function<void()> call;		// Callback con las coordenadas del raton
		bool pressed = false;

		int mouseButton = 0;

		MouseEvent(std::function<bool()> condition, std::function<void()> call, int button) :
			condition(condition), call(call), mouseButton(button) {
		}
	};

	/// @brief Estructura que maneja los cambios de estado del motor
	struct StateChanger {
		std::function<bool()> condition;
		std::function<void()> call;
		int state;

		StateChanger(std::function<bool()> condition, int state, std::function<void()> call = []() {}) :
			condition(condition), state(state), call(call) {}
	};

	/// @brief Singleton que se encarga de manejar los inputs del motor, desde atajos de teclado hasta eventos del raton. Se puede acceder a su instancia con inpM()
	class InputManager : public Singleton<InputManager>
	{
		friend class Singleton<InputManager>;
	private:
		int _currentState = CME_STATE_NORMAL;
		std::vector<Shortcut> _shortcuts;
		std::vector<StateChanger> _stateChangers;
		std::vector<MouseEvent> _mouseEvents;

		bool _isViewportHovered = false;

		float _mouseX = 0.0f;
		float _mouseY = 0.0f;

		float _viewportMouseX = 0.0f;
		float _viewportMouseY = 0.0f;

	public:
		~InputManager();

		/// @brief Procesa los inputs durante la ejecución
		void proccessInput();

		void addShortcut(Shortcut shortC) { _shortcuts.push_back(shortC); }
		void addStateChanger(StateChanger changer) { _stateChangers.push_back(changer); }
		void addMouseEvent(MouseEvent evt) { _mouseEvents.push_back(evt); }

		/// @brief Procesa el input del raton, se llama omo callback
		/// @param window LA ventana
		/// @param xpos La posicion del raton en el eje X
		/// @param ypos La posicion del raton en el eje Y
		static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

		void setViewportHovered(bool hovered) { _isViewportHovered = hovered; }
		bool isViewportHovered() const { return _isViewportHovered; }

		// cannot copy/move
		InputManager(InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		InputManager& operator=(InputManager&) = delete;
		InputManager& operator=(InputManager&&) = delete;

		float getMouseX() const { return _mouseX; }
		float getMouseY() const { return _mouseY; }

		float getViewportMouseX() const { return _viewportMouseX; }
		float getViewportMouseY() const { return _viewportMouseY; }
		void setViewportMouseX(float mouse) { _viewportMouseX = mouse; }
		void setViewportMouseY(float mouse) { _viewportMouseY = mouse; }

	private:
		InputManager();

		bool init();
	};

	inline InputManager& inpM() {
		return *InputManager::Instance();
	}
}

