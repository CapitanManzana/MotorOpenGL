#include "windows/ProjectWindow.h"
#include <EditorApp.h>
#include <windows/FileExplorerWindow.h>
#include <utils/logger.h>
#include <surface/Texture.h>
#include <managers/ResourceManager.h>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <cstdlib> // system()
#endif

namespace cme::editor {
	ProjectWindow::ProjectWindow(const char* name, std::shared_ptr<FileNode> fileN) : _rootNode(fileN), _selectedNode(fileN.get()), Window(name) { }

	void ProjectWindow::renderWindowContent() {
		ImGui::Text("Ventana de PRoyecto");
		ImGui::Separator();

		if (ImGui::BeginChild("##gallery", ImVec2(0, 0), false)) {
			float panelWidth = ImGui::GetContentRegionAvail().x;
			float cellWidth = _cellSize + 8.f;
			int columns = std::max(1, (int)(panelWidth / cellWidth));

			int i = 0;
			FileNode* nodeToChange;
			bool changeNode = false;
			for(auto it = _selectedNode->children.begin(); it != _selectedNode->children.end(); ++it) {
				FileNode& node = *it;
				int col = i % columns;
				if (col != 0) ImGui::SameLine();

				ImGui::BeginGroup();

				Texture* icon = nullptr;
				if (node.isDirectory) icon = rscrM().getTexture("folder");
				else if (node.fileType == FileType::Texture) icon = rscrM().getTexture(node.fullPath.stem().string());
				else icon = rscrM().getTexture("file");
				if (!icon) {
					LOG_WARN(std::format("Icono no encontrado para: {}", node.name));
					i++;
					ImGui::EndGroup();
					continue;
				}

				ImGui::PushID(i);
				ImGui::ImageButton("##img", (ImTextureID)(intptr_t)icon->id(), ImVec2(_cellSize, _cellSize));

				if (ImGui::IsItemHovered()) {
					if (ImGui::IsMouseDoubleClicked(0)) {
						if (node.isDirectory) {
							nodeToChange = &*it;
							changeNode = true;
						}
						else {
							openFile(node.fullPath, node.fileType);
						}
					}
				}

				ImGui::PopID();

				// Nombre truncado debajo
				std::string name = node.name;
				if (name.size() > 20) name = name.substr(0, 16) + "...";
				ImGui::TextUnformatted(name.c_str());

				// Tooltip con nombre completo
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", node.name.c_str());

				ImGui::EndGroup();

				i++;
			}

			if (changeNode) {
				_selectedNode = nodeToChange;
			}
		}

		ImGui::EndChild();
	}

	void ProjectWindow::changeToPath(fs::path path) {
		if (!_rootNode) return;

		FileNode* found = searchPathRecursive(*_rootNode, path);

		if (found) {
			_selectedNode = found;
		}
		else {
			LOG_WARN(std::format("Path no encontrado en el árbol: {}", path.string()));
			// Fallback a la raíz
			_selectedNode = _rootNode.get();
		}
	}

	FileNode* ProjectWindow::searchPathRecursive(FileNode& root, const fs::path& path) {
		if (root.fullPath == path) return &root;

		for (auto& child : root.children) {
			FileNode* found = searchPathRecursive(child, path);
			if (found) return found;
		}

		return nullptr;
	}

	void ProjectWindow::openFile(fs::path file, FileType type) {
		std::string fileName = file.string();
		switch (type)
		{
		case FileType::Scene:
			sceneM().loadScene(fileName);
			editor().createGizmos();
			break;
		case FileType::Texture:
			openWithDefaultApp(file);
			break;
		case FileType::Shader:
			openWithDefaultApp(file);
			break;
		default:
			break;
		}
	}


	void ProjectWindow::openWithDefaultApp(const fs::path& path) {
#ifdef _WIN32
#include <windows.h>
		ShellExecuteW(
			nullptr, L"open",
			path.wstring().c_str(),
			nullptr, nullptr,
			SW_SHOWNORMAL
		);

#elif __APPLE__
		std::string cmd = std::format("open \"{}\"", path.string());
		system(cmd.c_str());

#elif __linux__
		std::string cmd = std::format("xdg-open \"{}\"", path.string());
		system(cmd.c_str());
#endif
	}
}