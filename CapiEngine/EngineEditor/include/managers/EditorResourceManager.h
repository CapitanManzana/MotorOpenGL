#pragma once
#pragma once
#include <utils/Singleton.h>
#include <surface/Shader.h>
#include <surface/Texture.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace cme::editor {

	/// @brief Se encarga de la gesiton de recursos del editor. Utiliza el patron Singleton y se puede acceder a su instancia con rscrM()
	class EditorResourceManager : public Singleton<EditorResourceManager>
	{
		friend class Singleton<EditorResourceManager>;
	private:

	public:
		// cannot copy/move
		EditorResourceManager(EditorResourceManager&) = delete;
		EditorResourceManager(EditorResourceManager&&) = delete;
		EditorResourceManager& operator=(EditorResourceManager) = delete;
		EditorResourceManager& operator=(EditorResourceManager&&) = delete;
		
	private:
		bool init();
	};

	inline EditorResourceManager& editorRsc() {
		return *EditorResourceManager::Instance();
	}
}