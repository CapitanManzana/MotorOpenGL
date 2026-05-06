#include <project/ProjectLoader.h>
#include <managers/ResourceManager.h>
#include <EditorApp.h>

namespace cme::editor {
	bool ProjectLoader::init(fs::path enginePath, fs::path projectPath) {
		// Load engine resources
		loadFilesRecursive(enginePath / "resources");
		// load project resources
		loadFilesRecursive(projectPath / "assets");

		return true;
	}

	void ProjectLoader::loadFilesRecursive(const fs::path& root) {
		// Si no existe la ruta, salimos para evitar crasheos
		if (!fs::exists(root)) return;

		// Si nos pasan un archivo suelto en vez de una carpeta, lo procesamos directo
		if (!fs::is_directory(root)) {
			addResourceToCore(getFileType(root), root);
			return;
		}

		// Iterador recursivo mágico de C++17
		for (const auto& entry : fs::recursive_directory_iterator(root)) {
			// Solo procesamos si es un archivo de verdad (ignoramos carpetas)
			if (entry.is_regular_file()) {
				ResourceType type = getFileType(entry.path());
				addResourceToCore(type, entry.path());
			}
		}
	}

	void ProjectLoader::addResourceToCore(const ResourceType& type, const fs::path& file) {
		switch (type) {
		case ResourceType::Shader:
			rscrM().loadResource<Shader>(file);
			break;

		case ResourceType::Texture:
			rscrM().loadResource<Texture>(file);
			break;

		default:
			// Siempre es buena idea tener un default para cazar errores
			LOG_WARN("Se intentó cargar un tipo de recurso desconocido");
			break;
		}
	}


	ResourceType ProjectLoader::getFileType(const fs::path& file) {
		if (!fs::is_directory(file)) {
			auto extension = file.extension().string();

			if (std::count(SHADER_EXTENSIONS.begin(), SHADER_EXTENSIONS.end(), extension)) {
				return ResourceType::Shader;
			}
			else if (std::count(IMAGE_EXTENSIONS.begin(), IMAGE_EXTENSIONS.end(), extension)) {
				return ResourceType::Texture;
			}
		}

		return ResourceType::Unknown;
	}
}
