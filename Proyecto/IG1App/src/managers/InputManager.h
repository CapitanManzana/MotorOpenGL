#pragma once
#include <utils/Singleton.h>
#include <vector>
#include <functional>

#define CME_STATE_NORMAL 0
#define CME_STATE_VIEWPORT_MOVING 1

typedef std::function<void()> shortcutCall;

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
	public:
		~InputManager();

		void proccessInput();

		void addShortcut(Shortcut shortC) { _shortcuts.push_back(shortC); }
		void addStateChanger(StateChanger changer) { _stateChangers.push_back(changer); }

		// cannot copy/move
		InputManager(InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		InputManager& operator=(InputManager&) = delete;
		InputManager& operator=(InputManager&&) = delete;
	private:
		InputManager();

		bool init();
	};

	inline InputManager& inpM() {
		return *InputManager::Instance();
	}
}

