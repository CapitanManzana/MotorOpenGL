#pragma once
#include <windows/Window.h>
#include <memory>
#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

namespace cme::editor {
	struct FileNode;
	enum class FileType;

	/// @brief Ventana que se encarga de renderizar el viewport
	class ProjectWindow : public Window
	{
	private:
		std::shared_ptr<FileNode> _rootNode = nullptr;
		FileNode* _selectedNode = nullptr;

		int _cellSize = 80;
	public:
		WINDOW_ID(windowGroupID::PROJECT)
		ProjectWindow(const char* name, std::shared_ptr<FileNode> fileN);
		~ProjectWindow() {}

		void changeToPath(fs::path path);
	protected:
		void renderWindowContent() override;
		FileNode* searchPathRecursive(FileNode& root, const fs::path& path);

		void openFile(fs::path file, FileType type);
		void openWithDefaultApp(const fs::path& path);
	};
}