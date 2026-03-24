#pragma once
#include <unordered_map>
#include <utils/Singleton.h>
#include <core/Shader.h>

namespace cme {
	class Shader;

	/// @brief Se encarga de la gesiton de recursos del proyecto, desde shaders hasta imagenes. Utiliza el patron Singleton y se puede acceder a su instancia con rscrM()
	class ResourceManager : public Singleton<ResourceManager>
	{
		friend class Singleton<ResourceManager>;
	private:
		// ----- SHADERS -----
		const char* SHADERS_PATH = "assets/shaders";
		std::unordered_map<std::string, std::unique_ptr<Shader>> _shadersMap;
		std::vector<Shader*> _shaders;
		std::vector<std::string> _shaderNames;

	public:
		virtual ~ResourceManager();

		// cannot copy/move
		ResourceManager(ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		/// @brief Busca en los recursos el shader pedido mediante la key, que es el nombre del archivo sin la extensión
		/// @param key La clave del shader
		/// @return Devuelve un puntero inteligente al shader
		Shader* getShader(std::string key);
		/// @brief Busca todos los shaders cargados y los almacena en un vector
		/// @return Un vector de shaders
		std::vector<Shader*> getAllShaders();
		/// @brief Busca el nombre de todos los shader cargados
		/// @return Un vector de nombres
		std::vector<std::string> getAllShaderNames();

	private:
		ResourceManager() = default;

		/// @brief Inicializa el Resource Manager
		/// @return False si falla
		bool init();
	};

	/// @brief Devuelve la instancia de ResourceManager mediante el uso del patrón Singleton
	/// @return La instancia de ResourceManager
	inline ResourceManager& rscrM() {
		return *ResourceManager::Instance();
	}
}