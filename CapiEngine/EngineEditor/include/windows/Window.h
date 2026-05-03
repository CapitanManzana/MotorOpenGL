#pragma once
#include <imgui.h>
#include <core/ui/UIGroups.h>
#include <functional>
#include <string>

namespace cme::ui {
	/// @brief Interfaz para crear ventanas y añadirlas al render del CoreUI
	class Window
	{
	private:
		std::string _name;
		ImGuiWindowFlags _flags;
	public:
		Window(const char* name, ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse)
			: _name(name), _flags(flags) {}
		virtual ~Window() {}

		void render() {
			ImGui::Begin(_name.c_str(), 0, _flags);
			renderWindowContent();
			ImGui::End();
		}

	protected:
		virtual void renderWindowContent() = 0;
	};
}
