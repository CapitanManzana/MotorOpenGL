#include "UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <core/ui/Window.h>
#include <core/ui/windows/InspectorWindow.h>

namespace capiEngine::ui {
	UIManager::UIManager() {
		_windows.resize(groupID::NUM_GROUP);

		WindowFrame inspectorFrame = { 0,18,500,150 };
		addWindow<InspectorWindow>("Inspector", inspectorFrame);
	}

	UIManager::~UIManager() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	bool UIManager::initCoreUI(GLFWwindow* window) {
		if (!initImgui(window)) return false;

		return true;
	}

	bool UIManager::initImgui(GLFWwindow* window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
		if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) return false;
		if (!ImGui_ImplOpenGL3_Init("#version 330")) return false;

		return true;
	}

	void UIManager::render() const {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// RENDER DE LOS PANELES
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
				if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		for (auto& win : _windows) {
			if (win) win->render();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}