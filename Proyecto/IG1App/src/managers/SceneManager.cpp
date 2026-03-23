#include "SceneManager.h"
#include <core/Scene.h>
#include <ec/entity.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <core/mesh/CubeMesh.h>
#include <managers/ResourceManager.h>

SceneManager::~SceneManager() {
	delete _currentScene;
}

bool SceneManager::init() {
	return true;
}

void SceneManager::update() {
	if (!_currentScene) return;

	_currentScene->update();
}

void SceneManager::render() const {
	if (!_currentScene) return;

	_currentScene->render();
}

void SceneManager::loadScenes() {
	_currentScene = new Scene("Escena 1");
	auto cube = _currentScene->addGameObject(_currentScene, "Cube");
	auto tr = cube->addComponent<Transform>();
	cube->addComponent<MeshRenderer>(new CubeMesh(rscrM().getShader("default")));

	tr->setPosition(glm::vec3(0, 0, 0));
}

void saveActiveScene(std::string& path) {

}