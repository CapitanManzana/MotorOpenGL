#include <windows/FileExplorerWindow.h>
#include <windows/ProjectWindow.h>
#include <utils/logger.h>
#include <EditorApp.h>
#include <project/ProjectLoader.h>

namespace cme::editor {
	FileExplorerWindow::FileExplorerWindow(const char* name) : Window(name) {
		_rootPath = editor().projectPath() / "assets";
		LOG_INFO(std::format("ExeDir: {}", _rootPath.string()));

		if (!fs::exists(_rootPath)) {
			LOG_WARN(std::format("La carpeta assets no existe en: {}", _rootPath.string()));
			assert("No es compatible el project path");
		}

		_rootNode = std::make_shared<FileNode>(buildFileTree(_rootPath));
	}

	void FileExplorerWindow::renderWindowContent() {
		// Cabecera con ruta actual y botón de refresco
		ImGui::TextDisabled("%s", _rootPath.u8string().c_str());
		ImGui::SameLine();
		if (ImGui::SmallButton("Refresh")) {
			_rootNode = std::make_shared<FileNode>(buildFileTree(_rootPath));
		}

		ImGui::Separator();

		if (ImGui::BeginChild("##fileTree", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true)) {
			if (!_rootNode->name.empty())
				drawFileNode(*_rootNode, _selectedFile);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImVec2 pos = ImGui::GetCursorScreenPos();
		float height = ImGui::GetFrameHeight();
		ImGui::GetWindowDrawList()->AddLine(
			ImVec2(pos.x, pos.y),
			ImVec2(pos.x, pos.y + height),
			IM_COL32(100, 100, 100, 255), // color
			1.0f                          // grosor
		);
		ImGui::SetCursorScreenPos(ImVec2(pos.x + 5.0f, pos.y)); // desplaza el cursor

		ImGui::SameLine();

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

	FileNode FileExplorerWindow::buildFileTree(const fs::path root) {
		FileNode node;
		node.name = root.filename().string();
		node.fullPath = root;
		node.isDirectory = fs::is_directory(root);
		node.fileType = getFileType(root);

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

	void FileExplorerWindow::drawFileNode(const FileNode& node, fs::path& selectedFile) {
		if (!node.isDirectory) return;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;

		if (selectedFile == node.fullPath)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx(node.name.c_str(), flags);

		if (ImGui::IsItemClicked()) {
			selectedFile = node.fullPath;
			if (_clickDirCallback) _clickDirCallback(node.fullPath);
		}

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", node.fullPath.u8string().c_str());

		if (opened) {
			for (const auto& child : node.children)
				drawFileNode(child, selectedFile);
			ImGui::TreePop();
		}
	}

	FileType FileExplorerWindow::getFileType(fs::path file) {
		if (!fs::is_directory(file)) {
			auto extension = file.extension().string();

			if (extension == ".scene") {
				return FileType::Scene;
			}
			if (std::count(SHADER_EXTENSIONS.begin(), SHADER_EXTENSIONS.end(), extension)) {
				return FileType::Shader;
			}
			else if (std::count(IMAGE_EXTENSIONS.begin(), IMAGE_EXTENSIONS.end(), extension)) {
				return FileType::Texture;
			}
		}

		return FileType::None;
	}
}