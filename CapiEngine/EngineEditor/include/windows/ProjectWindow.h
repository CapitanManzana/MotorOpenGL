#pragma once
#include <windows/Window.h>
#include <memory>
#include <filesystem>
#include <optional>

namespace cme::editor {
	struct FileNode;

	/// @brief Ventana que se encarga de renderizar el viewport
	class ProjectWindow : public Window
	{
	private:
		std::shared_ptr<FileNode> _rootNode = nullptr;
		FileNode* _selectedNode;

		int _cellSize = 80;
	public:
		WINDOW_ID(windowGroupID::PROJECT)
		ProjectWindow(const char* name, std::shared_ptr<FileNode> fileN);
		~ProjectWindow() {}

	protected:
		void renderWindowContent() override;
	};
}