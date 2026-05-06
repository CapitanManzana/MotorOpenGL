#include <project/ProjectFileData.h>
#include <serialize/JsonSerializer.h>

namespace cme::editor {
	ProjectFileData::ProjectFileData(fs::path& projectPath) {
		JsonSerializer project;
		project.load(projectPath.string());

		_data.name = project.readString("name");
		_data.startScene = projectPath.parent_path().string() + "\\assets\\" + project.readString("startScene");
	}
}