#include "managers/ResourceManager.h"
#include <filesystem>
#include <utils/logger.h>
#include <surface/Texture.h>

namespace fs = std::filesystem;

namespace cme {
	ResourceManager::~ResourceManager() {}

	bool ResourceManager::init() {
		_shadersMap.clear();
		_shaders.clear();
		_shaderNames.clear();

		_texturesMap.clear();
		_texturesNames.clear();

		return true;
	}

	Shader* ResourceManager::getShader(std::string key) {
		auto it = _shadersMap.find(key);
		if (it != _shadersMap.end()) {
			return it->second.get();
		}

		LOG_WARN("Se intento obtener el shader '" << key << "', pero no existe");
		return nullptr;
	}

	Texture* ResourceManager::getTexture(std::string key) {
		auto it = _texturesMap.find(key);
		if (it != _texturesMap.end()) {
			return it->second.get();
		}

		LOG_WARN("Se intento obtener la textura '" << key << "', pero no existe");
		return nullptr;
	}

	void ResourceManager::loadShader(fs::path file) {

		if (!fs::exists(file) || fs::is_directory(file)) {
			LOG_ERROR(std::format("No existe el path de shaders: {}. Buscando desde: {}", file.string(), fs::current_path().string()));
		}

		auto extension = file.extension();
		auto name = file.stem();
		auto root = file.parent_path();

		// Recorremos cada archivo de la carpeta
		for (const auto& archivo : fs::directory_iterator(root)) {
			// Nos aseguramos de que sea un archivo y no una subcarpeta
			if (archivo.is_regular_file()) {
				std::string baseName = archivo.path().stem().string();
				std::string extension2 = archivo.path().extension().string();

				if (baseName == name && extension2 != extension) {
					// Si son pareja de shader
					if ((extension == ".vert" && extension2 == ".frag") || (extension == ".frag" && extension2 == ".vert")) {
						ShaderCreationData shaderData;
						if (extension == ".vert") {
							shaderData.vertexShaderPath = file.string();
							shaderData.fragmentShaderPath = archivo.path().string();
						}
						else {
							// ¡Aquí está la magia! Los invertimos:
							shaderData.vertexShaderPath = archivo.path().string();
							shaderData.fragmentShaderPath = file.string();
						}

						if (shaderData.isValid()) {
							LOG_INFO("Shader cargado correctamente: " << name);

							_shadersMap[name.string()] = std::make_unique<Shader>(shaderData);
							_shaders.push_back(_shadersMap[name.string()].get());
							_shaderNames.push_back(name.string());
						}
						else {
							LOG_WARN("Shader incompleto" << name << ".Falta el.vert o el.frag.\n");
						}
					}
				}
			}
		}
	}


	void ResourceManager::loadTexture(fs::path file) {
		if (!fs::exists(file) || fs::is_directory(file)) {
			LOG_ERROR(std::format("No existe el path de textures: {}. Buscando desde: {}", file.string(), fs::current_path().string()));
		}

		// Extraemos la información usando las utilidades de filesystem
		std::string extension = file.extension().string();
		std::string baseName = file.stem().string();
		std::string fullPath = file.string();

		_texturesMap[baseName] = std::make_unique<Texture>();
		_texturesMap[baseName]->load(fullPath);
		_texturesMap[baseName]->setName(baseName);
		_texturesNames.push_back(baseName);
	}

	std::vector<Shader*> ResourceManager::getAllShaders() {
		return _shaders;
	}

	std::vector<std::string> ResourceManager::getAllShaderNames() {
		return _shaderNames;
	}

	std::vector<std::string> ResourceManager::getAllTextureNames() {
		return _texturesNames;
	}
}