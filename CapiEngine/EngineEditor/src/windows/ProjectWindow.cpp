#include "windows/ProjectWindow.h"
#include <EditorApp.h>
#include <windows/FileExplorerWindow.h>
#include <utils/logger.h>
#include <surface/Texture.h>
#include <managers/ResourceManager.h>

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

				// Imagen clickeable
				//bool selected = (_selectedTex == tex);
				//if (selected) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.9f, 1.f));

				Texture* icon = nullptr;
				if (node.isDirectory) icon = rscrM().getTexture("folder");
				else icon = rscrM().getTexture("file");
				if (!icon) {
					LOG_WARN(std::format("Icono no encontrado para: {}", node.name));
					i++;
					ImGui::EndGroup();
					continue;
				}

				ImGui::PushID(i);
				if (ImGui::ImageButton("##img", (ImTextureID)(intptr_t)icon->id(), ImVec2(_cellSize, _cellSize))) {
					if (node.isDirectory) {
						nodeToChange = &*it;
						changeNode = true;
					}
				};
				ImGui::PopID();

				//if (selected) ImGui::PopStyleColor();

				// Nombre truncado debajo
				std::string name = node.name;
				if (name.size() > 10) name = name.substr(0, 9) + "...";
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
}