#include "core/Scene.h"
#include <ec/entity.h>
#include <core/Camera.h>
#include <managers/ResourceManager.h>
#include <ec/component.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <mesh/QuadMesh.h>
#include <mesh/CubeMesh.h>
#include <lighting/GlobalLight.h>
#include <managers/LightManager.h>

namespace cme {
	Scene::Scene(std::string name) : _name(name) {
		_cam = new Camera();
		_globalLight = new GlobalLight();
	}

	Scene::~Scene() {
		delete _cam;
		delete _globalLight;
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
							e.reset();
							return true;
						}
					}),
				grpEnts.end());
		}
	}

	void Scene::render() const {
		for (int i = 0; i < ec::ent::maxGroupLayer; i++) {
			for (auto& gObj : _gameObjectsByGroup[i]) {
				if (gObj && gObj->active()) gObj->render();
			}
		}

		for (auto& go : _gizmos) {
			go->render();
		}
	}

	void Scene::update() {
		for (int i = 0; i < ec::ent::maxGroupLayer; i++) {
			for (auto& gObj : _gameObjectsByGroup[i]) {
				if (gObj->active()) gObj->update();
			}
		}
	};

	std::shared_ptr<ec::Entity> Scene::addGameObject(Scene* scene, std::string name, ec::ent::groupID grID) {
		auto shPtr = std::make_shared<ec::Entity>(grID, scene, name);
		_gameObjectsByGroup[grID].push_back(shPtr);
		return shPtr;
	}

	ec::entity_t Scene::addGizmos() {
		auto shPtr = std::make_shared<ec::Entity>((ec::ent::groupID)0, this, "");
		_gizmos.push_back(shPtr);
		return shPtr;
	}

	void Scene::addCubeToScene() {
		auto c = addGameObject(this, "Cube", ec::ent::None);
		auto tr = c->addComponent<Transform>();
		tr->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		tr->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		tr->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));

		c->addComponent<MeshRenderer>(new CubeMesh(rscrM().getShader("default")));
	}

	// --------- SERIALIZACION ------------

	void Scene::serialize(JsonSerializer& s) const {
		s.write("name", _name);
		s.beginArray("entities_groups");
		for (int i = 0; i < ec::ent::maxGroupLayer; i++) {
			s.pushObjectToArray();
			s.write("group", i);
			s.beginArray("entities");
			for (auto& gObj : _gameObjectsByGroup[i]) {
				gObj->serialize(s);
			}
			s.endScope(); // El del array de entidades
			s.endScope(); // El array de grupos
		}
		s.endScope(); // El array de grupos
		

		s.beginScope("globalLight");
		_globalLight->serialize(s);
		s.endScope();
	}

	void Scene::deserialize(JsonSerializer& s) {
		lightM().clearLights();

		_name = s.readString("name");
		s.beginArray("entities_groups");
		size_t numGroups = s.getArraySize();

		if (numGroups > ec::ent::maxGroupLayer) {
			LOG_ERROR("Hay mas grupos de entidades de los permitidos en el archivo de escena a cargar");
			return;
		}

		for (size_t i = 0; i < numGroups; i++) {
			s.enterElement(i);
			ec::groupID_t grpID = s.readInt("group");
			s.beginArray("entities");

			size_t numEntities = s.getArraySize();
			for (size_t j = 0; j < numEntities; j++) {
				s.enterElement(j);
				std::string entityName = s.readString("name");
				auto ent = addGameObject(this, entityName, (ec::ent::groupID)grpID);
				ent->deserialize(s);

				s.endScope(); // Salimos de la entidad j
			}
			s.endScope(); // Salimos del array entities
			s.endScope(); // Salimos del grupo 1
		}

		s.endScope();

		s.beginScope("globalLight");
		_globalLight->deserialize(s);
		s.endScope();
	}
}