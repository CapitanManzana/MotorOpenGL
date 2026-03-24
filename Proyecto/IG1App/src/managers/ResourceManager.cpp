#include "ResourceManager.h"
#include <filesystem>
#include <utils/logger.h>

namespace fs = std::filesystem;

ResourceManager::~ResourceManager() {}

bool ResourceManager::init() {
    std::unordered_map<std::string, ShaderCreationData> foundShaders;

    if (!fs::exists(SHADERS_PATH) || !fs::is_directory(SHADERS_PATH)) {
        LOG_ERROR("No existe el path de shaders: " << SHADERS_PATH << ". Buscando desde: " << fs::current_path());
        return false;
    }

    // Recorremos cada archivo de la carpeta
    for (const auto& archivo : fs::directory_iterator(SHADERS_PATH)) {
        // Nos aseguramos de que sea un archivo y no una subcarpeta
        if (archivo.is_regular_file()) {
            // Extraemos la información usando las utilidades de filesystem
            std::string extension = archivo.path().extension().string(); // ej: ".vert"
            std::string baseName = archivo.path().stem().string();     // ej: "basico"
            std::string fullPath = archivo.path().string();          // ej: "assets/shaders/basico.vert"

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
        }
        else {
            LOG_WARN("Shader incompleto" << name << ".Falta el.vert o el.frag.\n");
        }
    }
    
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

std::vector<Shader*> ResourceManager::getAllShaders() {
    std::vector<Shader*> out;
    for (const auto& par : _shadersMap) {
        if (par.second) out.push_back(par.second.get());
    }

    return out;
}