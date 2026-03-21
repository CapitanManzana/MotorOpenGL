#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <core/ui/UIGroups.h>
#include <functional>
#include <string>

namespace capiEngine::ui {
	struct WindowFrame {
		float x = 0.0f;
		float y = 0.0f;
		float w = 10.0f;
		float h = 10.0f;

		WindowFrame() = default;
		WindowFrame(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	};

	/// @brief Interfaz para crear ventanas y añadirlas al render del CoreUI
	class Window
	{
	private:
		WindowFrame _frame;
		std::string _name;
		ImGuiWindowFlags _flags;
	public:
		Window(const char* name, WindowFrame frame = WindowFrame(), ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)
			: _name(name), _flags(flags), _frame(frame) {}

		void render() const {
			ImGui::SetNextWindowPos(ImVec2(_frame.x, _frame.y), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(_frame.w, _frame.h), ImGuiCond_Once);
			ImGui::Begin(_name.c_str(), 0, _flags);
			renderWindowContent();
			ImGui::End();
		}

	protected:
		virtual void renderWindowContent() const = 0;
	};
}
