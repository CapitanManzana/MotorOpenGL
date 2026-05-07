#pragma once
#include <windows/Window.h>
#include <memory>
#include <filesystem>
#include <functional>

namespace cme::editor {
	namespace fs = std::filesystem;

	enum class FileType {
		None, Scene, Texture, Shader
	};

	struct FileNode {
		std::string name;
		fs::path fullPath;
		bool isDirectory;

		std::vector<FileNode> children;
		FileType fileType;
	};

	class ProjectWindow;

	/// @brief Ventana que se encarga de renderizar el viewport
	class FileExplorerWindow : public Window
	{
	private:
		std::shared_ptr<FileNode> _rootNode;
		fs::path _rootPath;
		fs::path _selectedFile;

		std::function<void(fs::path)> _clickDirCallback;
	public:
		WINDOW_ID(windowGroupID::FILE_EXPLORER)
			FileExplorerWindow(const char* name);
		~FileExplorerWindow() {}

		std::shared_ptr<FileNode> fileNode() { return _rootNode; }
		void cickCallback(std::function<void(fs::path)> call) { _clickDirCallback = call; }
	protected:
		void renderWindowContent() override;

	private:
		FileNode buildFileTree(const fs::path root);
		void drawFileNode(const FileNode& node, fs::path& selectedFile);

		FileType getFileType(fs::path file);
	};
}

