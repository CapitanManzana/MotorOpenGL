#include "ResourceManager.h"
#include <filesystem>
#include <utils/logger.h>
#include <core/Shader.h>
#include <core/Mesh.h>

namespace fs = std::filesystem;

namespace cme {
	ResourceManager::~ResourceManager() {}

	bool ResourceManager::init() {
		loadShaders();
		return true;
	}

	Shader* ResourceManager::getShader(const std::string& name) {
		auto it = _shadersMap.find(name);
		if (it != _shadersMap.end()) {
			return it->second.get();
		}
		return nullptr;
	}

	std::vector<std::string> ResourceManager::getAllShaderNames() {
		return _shaderNames;
	}

	Mesh* ResourceManager::getMesh(const std::string& name) {
		auto it = _meshesMap.find(name);
		if (it != _meshesMap.end()) {
			return it->second.get();
		}
		return nullptr;
	}

	void ResourceManager::registerMesh(const std::string& name, std::shared_ptr<Mesh> mesh) {
		_meshesMap[name] = mesh;
	}

	void ResourceManager::loadShaders() {
		if (!fs::exists(_shadersPath)) {
			return;
		}

		for (const auto& entry : fs::directory_iterator(_shadersPath)) {
			if (entry.is_regular_file() && entry.path().extension() == ".vert") {
				std::string name = entry.path().stem().string();
				_shaderNames.push_back(name);
				
				std::string vertPath = entry.path().string();
				std::string fragPath = entry.path().parent_path().string() + "/" + name + ".frag";
				
				if (fs::exists(fragPath)) {
					_shadersMap[name] = std::make_unique<Shader>(ShaderCreationData(vertPath, fragPath, name));
				}
			}
		}
	}
}
