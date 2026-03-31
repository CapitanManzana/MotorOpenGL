#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <utils/Singleton.h>

#include <core/Shader.h>

namespace cme {
	class Mesh;

	class ResourceManager : public Singleton<ResourceManager>
	{
		friend class Singleton<ResourceManager>;
	public:
		ResourceManager() = default;
		virtual ~ResourceManager();

		bool init();

		// cannot copy/move
		ResourceManager(ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		/// @brief Busca en los recursos el shader pedido mediante la key, que es el nombre del archivo sin la extensión
		/// @param key La clave del shader
		/// @return Devuelve un puntero inteligente al shader
		Shader* getShader(const std::string& key);
		/// @brief Busca todos los shaders cargados y los almacena en un vector
		/// @return Un vector de shaders
		std::vector<Shader*> getAllShaders();
		/// @brief Busca el nombre de todos los shader cargados
		/// @return Un vector de nombres
		std::vector<std::string> getAllShaderNames();

		Mesh* getMesh(const std::string& name);
		void registerMesh(const std::string& name, std::shared_ptr<Mesh> mesh);

	private:
		std::string _shadersPath = "assets/shaders";
		std::unordered_map<std::string, std::unique_ptr<Shader>> _shadersMap;
		std::vector<std::string> _shaderNames;
		std::unordered_map<std::string, std::shared_ptr<Mesh>> _meshesMap;

		void loadShaders();
	};

	inline ResourceManager& rscrM() {
		return *ResourceManager::Instance();
	}
}