#pragma once
#include <utils/Singleton.h>

namespace cme {
	class Scene;

	class SceneManager : public Singleton<SceneManager>
	{
		friend class Singleton<SceneManager>;
	private:
		Scene* _currentScene = nullptr;
	public:
		virtual ~SceneManager();

		void update();
		void render() const;

		void loadScenes(std::string& path);
		void saveActiveScene(std::string& path) const;

		/// @brief Devuelve la escena activa
		/// @return Puntero a la escena
		Scene* activeScene() { return _currentScene; }

		// cannot copy/move
		SceneManager(SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;
	private:
		SceneManager() = default;

		bool init();
	};

	inline SceneManager& sceneM() {
		return *SceneManager::Instance();
	}
}
