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
		FileNode* _rootNode = nullptr;
		FileNode* _selectedNode = nullptr;

		int _cellSize = 80;
	public:
		WINDOW_ID(windowGroupID::PROJECT)
		ProjectWindow(const char* name, FileNode& fileN);

	protected:
		void renderWindowContent() override;
	};
}