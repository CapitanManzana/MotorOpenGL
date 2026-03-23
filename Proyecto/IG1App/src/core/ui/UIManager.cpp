#include "UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <core/ui/Window.h>
#include <core/ui/windows/InspectorWindow.h>
#include <core/ui/windows/ViewportWindow.h>
#include <core/ui/windows/SceneWindow.h>
#include <core/ui/windows/ConsoleWindow.h>
#include <utils/logger.h>
namespace capiEngine::ui {
	UIManager::UIManager() {
		_windows.resize(windowGroupID::NUM_GROUP);

		auto inspector = addWindow<InspectorWindow>("Inspector");
		addWindow<ViewportWindow>("Viewport");
		auto hierarchy = addWindow<SceneWindow>("Scene");
		auto console = addWindow<ConsoleWindow>("Console");

		hierarchy->setCallback([inspector](ec::entity_t e) {
			inspector->changeDisplayEntity(e);
		});

		logger().setCallback([console](const std::string& msg) {
			console->addLog(msg);
		});
	}

	UIManager::~UIManager() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyPlatformWindows();
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
		io.IniFilename = "imgui.ini";
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) return false;
		if (!ImGui_ImplOpenGL3_Init("#version 330")) return false;

		return true;
	}

	void UIManager::bind() const {
		auto& viewport = _windows[windowGroupID::VIEWPORT];
		if (viewport.get()) {
			ViewportWindow& vp = static_cast<ViewportWindow&>(*viewport);
			vp.bindFBO();
		}
	}

	void UIManager::unbind() const {
		auto& viewport = _windows[windowGroupID::VIEWPORT];
		if (viewport.get()) {
			ViewportWindow& vp = static_cast<ViewportWindow&>(*viewport);
			vp.unbindFBO();
		}
	}

	void UIManager::render() const {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// RENDER DE LOS PANELES

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_MenuBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("DockSpace", nullptr, flags);
		ImGui::PopStyleVar();
		ImGui::DockSpace(ImGui::GetID("MainDockSpace"));
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
		ImGui::End();

		for (auto& win : _windows) {
			if (win) win->render();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}