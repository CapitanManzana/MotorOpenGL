#include "managers/UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <windows/Window.h>
#include <windows/InspectorWindow.h>
#include <windows/ViewportWindow.h>
#include <windows/SceneWindow.h>
#include <windows/ConsoleWindow.h>
#include <windows/LightingWindow.h>

#include <managers/SceneManager.h>

#include <utils/logger.h>
#include <utils/FileExplorer.h>

namespace cme::editor {
	UIManager::UIManager() {
		_windows.resize(windowGroupID::NUM_GROUP);
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

	void UIManager::start() {
		auto inspector = addWindow<InspectorWindow>("Inspector");
		addWindow<ViewportWindow>("Viewport");
		auto hierarchy = addWindow<SceneWindow>("Scene");
		auto console = addWindow<ConsoleWindow>("Console");
		addWindow<LightingWindow>("Lighting");

		hierarchy->setCallback([inspector](std::weak_ptr<ec::Entity> e) {
			inspector->changeDisplayEntity(e);
			});

		logger().setCallback([console](const std::string& msg) {
			console->addLog(msg);
			});

		if (auto& viewport = _windows[windowGroupID::VIEWPORT]) {
			ViewportWindow& vp = static_cast<ViewportWindow&>(*viewport);
			vp.initialResize();
		}
	}

	void UIManager::render() const {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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

		renderMenuBar();

		ImGui::End();

		for (auto& win : _windows) {
			if (win) win->render();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void UIManager::renderMenuBar() const {
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
					FileExplorer fe;
					std::string path = fe.fileDialog(FileDialogMode::Save);
					sceneM().saveActiveScene(path);
				}

				if (ImGui::MenuItem("Load Scene", "Ctrl+L")) {
					FileExplorer fe;
					std::string path = fe.fileDialog(FileDialogMode::Open);
					sceneM().loadScene(path);
				}
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

			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::MenuItem("Cube")) {
					_createCubeCallback();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}