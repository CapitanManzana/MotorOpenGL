#include "windows/ProjectWindow.h"
#include <EditorApp.h>

#include <utils/logger.h>

namespace cme::editor {
	ProjectWindow::ProjectWindow(const char* name) : Window(name) {
		_rootPath = EditorApp::getExeDir() / "assets";
		LOG_INFO(std::format("ExeDir: {}", _rootPath.string()));

		if (!fs::exists(_rootPath)) {
			LOG_WARN(std::format("La carpeta assets no existe en: {}", _rootPath.string()));
			_rootPath = EditorApp::getExeDir(); // fallback a la carpeta del exe
		}

		_rootNode = buildFileTree(_rootPath);
	}

	void ProjectWindow::renderWindowContent() {
		// Cabecera con ruta actual y botón de refresco
		ImGui::TextDisabled("%s", _rootPath.u8string().c_str());
		ImGui::SameLine();
		if (ImGui::SmallButton("Refresh")) {
			_rootNode = buildFileTree(_rootPath);
		}

		ImGui::Separator();

		if (ImGui::BeginChild("##fileTree", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true)) {
			if (!_rootNode.name.empty())
				drawFileNode(_rootNode, _selectedFile);
		}
		ImGui::EndChild();

		ImGui::Separator();
		if (!_selectedFile.empty()) {
			ImGui::TextUnformatted("Selected:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.f),
				"%s", _selectedFile.filename().string().c_str());
		}
		else {
			ImGui::TextDisabled("No file selected");
		}
	}

	FileNode ProjectWindow::buildFileTree(const fs::path root) {
		FileNode node;
		node.name = root.filename().string();
		node.fullPath = root;
		node.isDirectory = fs::is_directory(root);

		if (node.isDirectory) {
			std::vector<FileNode> dirs, files;
			std::error_code ec;
			for (const auto& entry : fs::directory_iterator(root)) {
				if (ec) {
					LOG_WARN(std::format("Error iterando: {}", ec.message()));
					break;
				}

				FileNode child = buildFileTree(entry);
				if (child.isDirectory) dirs.push_back(child);
				else files.push_back(child);
			}

			auto byName = [](const FileNode& a, const FileNode& b) {
				return a.name < b.name;
				};

			std::sort(dirs.begin(), dirs.end(), byName);
			std::sort(files.begin(), files.end(), byName);

			node.children.insert(node.children.end(), dirs.begin(), dirs.end());
			node.children.insert(node.children.end(), files.begin(), files.end());
		}

		return node;
	}

	void ProjectWindow::drawFileNode(const FileNode& node, fs::path& selectedFile) {
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;

		if (!node.isDirectory)
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		if (selectedFile == node.fullPath)
			flags |= ImGuiTreeNodeFlags_Selected;

		// Icono simple según tipo
		const char* icon = node.isDirectory ? "📁 " : "📄 ";
		std::string label = icon + node.name;

		bool opened = ImGui::TreeNodeEx(label.c_str(), flags);

		if (ImGui::IsItemClicked() && !node.isDirectory)
			selectedFile = node.fullPath;

		// Tooltip con la ruta completa
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", node.fullPath.string().c_str());

		if (node.isDirectory && opened) {
			for (const auto& child : node.children)
				drawFileNode(child, selectedFile);
			ImGui::TreePop();
		}
	}
}