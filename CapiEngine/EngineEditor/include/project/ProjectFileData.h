#pragma once
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace cme::editor {
	struct ProjecData {
		std::string name;
		std::string startScene;
	};

	class ProjectFileData {
	private:
		ProjecData _data;
	public:
		ProjectFileData(fs::path& projectPath);

		ProjecData& projectData() { return _data; }
	};
}