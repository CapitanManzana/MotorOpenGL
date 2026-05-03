#pragma once
#include <utils/Singleton.h>

namespace cme {
	class Scene;

	/// @brief Singleton que se encarga de gestionar la escena activa, permitiendo cargar y guardar escenas, así como actualizar y renderizar la escena activa. Se puede acceder a su instancia con sceneM()
	class SceneManager : public Singleton<SceneManager>
	{
		friend class Singleton<SceneManager>;
	private:
		Scene* _currentScene = nullptr;
	public:
		virtual ~SceneManager();

		void update();
		void render() const;
		void start();

		void loadScene(std::string& path);
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
