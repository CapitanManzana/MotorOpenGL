#pragma once
#include <windows/Window.h>
#include <memory>
#include <filesystem>

namespace cme::editor {

	namespace fs = std::filesystem;

	struct FileNode {
		std::string name;
		fs::path fullPath;
		bool isDirectory;
		std::vector<FileNode> children;
	};

	/// @brief Ventana que se encarga de renderizar el viewport
	class ProjectWindow : public Window
	{
	private:
		FileNode _rootNode;
		fs::path _rootPath;
		fs::path _selectedFile;

	public:
		WINDOW_ID(windowGroupID::PROJECT)
		ProjectWindow(const char* name);

	protected:
		void renderWindowContent() override;

	private:
		FileNode buildFileTree(const fs::path root);
		void drawFileNode(const FileNode& node, fs::path& selectedFile);

		fs::path getExeDir();
	};
}