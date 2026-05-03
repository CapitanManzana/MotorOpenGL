#pragma once
#include <windows/Window.h>
#include <string>
#include <vector>

namespace cme::editor {
	/// @brief Ventana que sirve para mostrar los logs del motor.
	class ConsoleWindow : public Window
	{
	private:
		std::vector<std::string> _logs;
	public:
		WINDOW_ID(windowGroupID::CONSOLE)
		ConsoleWindow(const char* name, ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse) :
			Window(name, flags) { }

		~ConsoleWindow() {
			std::vector<std::string>().swap(_logs);
			_logs.clear();
		}

		void addLog(std::string log) { _logs.push_back(log); }

	protected:
		virtual void renderWindowContent() override;
	};
}

