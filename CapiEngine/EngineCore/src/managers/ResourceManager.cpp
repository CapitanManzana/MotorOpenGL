#include "ResourceManager.h"
#include <filesystem>
#include <utils/logger.h>
#include <core/Texture.h>

namespace fs = std::filesystem;

namespace cme {
    ResourceManager::~ResourceManager() {}

    bool ResourceManager::init() {
        loadShaders();
        loadTextures();

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

    void ResourceManager::loadShaders() {
        std::unordered_map<std::string, ShaderCreationData> foundShaders;

        if (!fs::exists(SHADERS_PATH) || !fs::is_directory(SHADERS_PATH)) {
            LOG_ERROR("No existe el path de shaders: " << SHADERS_PATH << ". Buscando desde: " << fs::current_path());
        }

        // Recorremos cada archivo de la carpeta
        for (const auto& archivo : fs::directory_iterator(SHADERS_PATH)) {
            // Nos aseguramos de que sea un archivo y no una subcarpeta
            if (archivo.is_regular_file()) {
                // Extraemos la información usando las utilidades de filesystem
                std::string extension = archivo.path().extension().string(); // ej: ".vert"
                std::string baseName = archivo.path().stem().string();     // ej: "basico"
                std::string fullPath = archivo.path().string();          // ej: "assets/shaders/basico.vert"
                foundShaders[baseName].name = baseName;

                // 2. Clasificamos el archivo según su extensión
                if (extension == ".vert") {
                    foundShaders[baseName].vertexShaderPath = fullPath;
                }
                else if (extension == ".frag") {
                    foundShaders[baseName].fragmentShaderPath = fullPath;
                }
            }
        }

        // 3. Revisamos qué hemos encontrado y los construimos
        for (const auto& par : foundShaders) {
            const std::string& name = par.first;
            ShaderCreationData shaderData = par.second;

            if (shaderData.isValid()) {
                LOG_INFO("Shader cargado correctamente: " << name);

                _shadersMap[name] = std::make_unique<Shader>(shaderData);
                _shaders.push_back(_shadersMap[name].get());
                _shaderNames.push_back(name);
            }
            else {
                LOG_WARN("Shader incompleto" << name << ".Falta el.vert o el.frag.\n");
            }
        }
    }

    void ResourceManager::loadTextures() {
        if (!fs::exists(TEXTURES_PATH) || !fs::is_directory(TEXTURES_PATH)) {
            LOG_ERROR("No existe el path de textures: " << TEXTURES_PATH << ". Buscando desde: " << fs::current_path());
        }

        for (const auto& archivo : fs::directory_iterator(TEXTURES_PATH)) {
            // Nos aseguramos de que sea un archivo y no una subcarpeta
            if (archivo.is_regular_file()) {
                // Extraemos la información usando las utilidades de filesystem
                std::string extension = archivo.path().extension().string();
                std::string baseName = archivo.path().stem().string();
                std::string fullPath = archivo.path().string();
                
                _texturesMap[baseName] = std::make_unique<Texture>();
                _texturesMap[baseName]->load(fullPath);
                _texturesMap[baseName]->setName(baseName);
                _texturesNames.push_back(baseName);
            }
        }
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