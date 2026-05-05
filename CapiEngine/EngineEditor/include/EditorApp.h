#pragma once
#include <GLApplication.h>
#include <utils/Singleton.h>
#include <managers/SceneManager.h>
#include <managers/InputManager.h>

#include <core/Scene.h>
#include <utils/logger.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace cme::editor {
	class UIManager;

	class EditorApp : public Singleton<EditorApp>{
		friend class Singleton<EditorApp>;
	private:
		std::shared_ptr<UIManager> _ui;
		fs::path _projectPath;
		fs::path _enginePath;
	public:
		~EditorApp() {
			if (GLApplication::HasInstance()) {
				GLApplication::Release();
			}
		}

		EditorApp(EditorApp&) = delete;
		EditorApp(EditorApp&&) = delete;
		EditorApp& operator=(EditorApp&) = delete;
		EditorApp& operator=(EditorApp&&) = delete;

		void run();

		void createShortcuts();
		void stateChangers();

		void createGizmos();

		// Metodos de direcciones
		static fs::path getExeDir();
		static fs::path getEngineDataPath();

		fs::path projectPath() { return _projectPath; }
		fs::path enginePath() { return _enginePath; }

	private:
		bool init(fs::path enginePath, fs::path projectPath);
	};

	inline EditorApp& editor() {
		return *EditorApp::Instance();
	}
}
