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
	class ProjectFileData;

	class EditorApp : public Singleton<EditorApp>{
		friend class Singleton<EditorApp>;
	private:
		std::shared_ptr<UIManager> _ui;
		fs::path _projectPath;
		fs::path _enginePath;

		fs::path _projectFile;

		ProjectFileData* _projectData;
	public:
		~EditorApp();

		EditorApp(EditorApp&) = delete;
		EditorApp(EditorApp&&) = delete;
		EditorApp& operator=(EditorApp&) = delete;
		EditorApp& operator=(EditorApp&&) = delete;

		void run();

		void createShortcuts();
		void stateChangers();

		void createGizmos();

		// Metodos de direcciones
		fs::path getEngineDataPath();

		const fs::path& projectPath() const { return _projectPath; }
		const fs::path& enginePath() const { return _enginePath; }
		const fs::path& projectFile() const { return _projectFile; }

		ProjectFileData* projectData() { return _projectData; }
	private:
		EditorApp() = default;
		bool init(fs::path enginePath, fs::path projectPath, fs::path projFile);
	};

	inline EditorApp& editor() {
		return *EditorApp::Instance();
	}
}
