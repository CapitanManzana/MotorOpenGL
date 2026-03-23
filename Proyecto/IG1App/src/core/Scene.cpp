#include "Scene.h"
#include <ec/entity.h>
#include <core/Camera.h>
#include <managers/ResourceManager.h>
#include <ec/component.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <core/mesh/QuadMesh.h>

Scene::Scene(std::string name) : _name(name) {
	_cam = new Camera();
	createGrid();
}

Scene::~Scene() {

	for (int i = 0; i < ec::ent::maxGroupLayer; i++) {
		for (ec::Entity* gObj : _gameObjectsByGroup[i]) {
			delete gObj;
		}
	}

	for (auto& go : _gizmos) {
		delete go;
	}

	delete _cam;
}

void Scene::refresh() {
	for (ec::groupID_t gId = 0; gId < ec::ent::maxGroupLayer; gId++) {
		auto& grpEnts = _gameObjectsByGroup[gId];
		grpEnts.erase(
			std::remove_if(grpEnts.begin(), grpEnts.end(),
				[](ec::entity_t e) {
					if (e->isAlive()) {
						return false;
					}
					else {
						delete e;
						return true;
					}
				}),
			grpEnts.end());
	}
}

void Scene::render() const {
	for (int i = 0; i < ec::ent::maxGroupLayer; i++) {
		for (ec::entity_t gObj : _gameObjectsByGroup[i]) {
			if (gObj->active()) gObj->render();
		}
	}

	for (auto& go : _gizmos) {
		go->render();
	}
}

void Scene::update() {
	for (int i = 0; i < ec::ent::maxGroupLayer; i++) {
		for (ec::entity_t gObj : _gameObjectsByGroup[i]) {
			if (gObj->active()) gObj->update();
		}
	}
};

ec::entity_t Scene::addGameObject(Scene* scene, std::string name, ec::ent::groupID grID) {
	ec::entity_t gObj = new ec::Entity(grID, scene, name);
	_gameObjectsByGroup[grID].push_back(gObj);
	return gObj;
}

ec::entity_t Scene::addGizmos() {
	ec::entity_t gObj = new ec::Entity((ec::ent::groupID)0, this, "");
	_gizmos.push_back(gObj);
	return gObj;
}

void Scene::createGrid() {
	Shader* gridShader = rscrM().getShader("grid");
	// 2. Crear la entidad
	ec::entity_t gridEntity = addGizmos();

	// 3. Añadirle el Transform (escala grande, ej. 100x100)
	auto tr = gridEntity->addComponent<Transform>();
	tr->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	tr->setScale(glm::vec3(500.0f, 1.0f, 500.0f)); // Un plano muy grande
	tr->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f)); // Rotarlo para que acueste en Y=0 (depende de tu QuadMesh)

	// 4. Añadir tu QuadMesh
	gridEntity->addComponent<MeshRenderer>(new QuadMesh(gridShader));
	//gridShader->setUniform("model", tr->getModelMatrix());
	//gridShader->setUniform("cameraPos", _cam->getPosition());
}