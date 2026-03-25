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

	struct  MouseEvent {
		std::function<bool()> condition;			// Condicion a ocurrir
		std::function<void(float,float)> call;		// Callback con las coordenadas del raton

		MouseEvent(std::function<bool()> condition, std::function<void(float, float)> call) :
			condition(condition), call(call) {
		}
	};

	struct StateChanger {
		std::function<bool()> condition;
		std::function<void()> call;
		int state;

		StateChanger(std::function<bool()> condition, int state, std::function<void()> call = []() {}) :
			condition(condition), state(state), call(call) {}
	};

	class InputManager : public Singleton<InputManager>
	{
		friend class Singleton<InputManager>;
	private:
		int _currentState = CME_STATE_NORMAL;
		std::vector<Shortcut> _shortcuts;
		std::vector<StateChanger> _stateChangers;
		std::vector<MouseEvent> _mouseEvents;

		bool _isViewportHovered = false;
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
	private:
		InputManager();

		void createShortcuts();
		bool init();
	};

	inline InputManager& inpM() {
		return *InputManager::Instance();
	}
}

