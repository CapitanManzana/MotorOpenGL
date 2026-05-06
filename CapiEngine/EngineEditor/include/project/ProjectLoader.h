#pragma once
#pragma once
#include <utils/Singleton.h>
#include <filesystem>
#include <unordered_set>
#include <variant>
#include <array>

namespace fs = std::filesystem;

namespace cme::editor {
	
	constexpr std::array<std::string_view, 5> IMAGE_EXTENSIONS = {
		".png", ".jpg", ".jpeg", ".bmp", ".tga"
	};

	constexpr std::array<std::string_view, 5> SHADER_EXTENSIONS = {
		".frag", ".vert"
	};

	enum class ResourceType { Shader, Texture, Unknown };

	/// @brief Se encarga de la gesiton de recursos del editor. Utiliza el patron Singleton y se puede acceder a su instancia con rscrM()
	class ProjectLoader : public Singleton<ProjectLoader>
	{
		friend class Singleton<ProjectLoader>;
	public:
		// cannot copy/move
		ProjectLoader(ProjectLoader&) = delete;
		ProjectLoader(ProjectLoader&&) = delete;
		ProjectLoader& operator=(ProjectLoader) = delete;
		ProjectLoader& operator=(ProjectLoader&&) = delete;
		
	private:
		ProjectLoader() = default;
		bool init(fs::path enginePath, fs::path projectPath);

		void loadFilesRecursive(const fs::path& root);
		void addResourceToCore(const ResourceType& type, const fs::path& file);
		ResourceType getFileType(const fs::path& file);
	};

	inline ProjectLoader& projLoader() {
		return *ProjectLoader::Instance();
	}
}