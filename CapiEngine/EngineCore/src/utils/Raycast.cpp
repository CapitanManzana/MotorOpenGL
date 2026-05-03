#include "Raycast.h"
#include <algorithm>
#include <managers/InputManager.h>
#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <core/Mesh.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <ec/component.h>

namespace cme {
	bool Raycast::rayIntersectsWithObj(glm::vec3 boxMin, glm::vec3 boxMax, float& collisionDist) {
		glm::vec3 invDir = 1.0f / _rayDirection;

		float t1 = (boxMin.x - _rayOrigin.x) * invDir.x;
		float t2 = (boxMax.x - _rayOrigin.x) * invDir.x;
		float t3 = (boxMin.y - _rayOrigin.y) * invDir.y;
		float t4 = (boxMax.y - _rayOrigin.y) * invDir.y;
		float t5 = (boxMin.z - _rayOrigin.z) * invDir.z;
		float t6 = (boxMax.z - _rayOrigin.z) * invDir.z;

		float tMin = std::max(std::max(std::min(t1,t2), std::min(t3,t4)), std::min(t5,t6));
		float tMax = std::min(std::min(std::max(t1,t2), std::max(t3,t4)), std::max(t5,t6));

		if (tMax < 0 || tMin > tMax) {
			return false;
		}

		collisionDist = tMin;
		return true;
	}

	void Raycast::calculateWorldAABB(MeshRenderer* mesh, Transform* transform, glm::vec3& outWorldMin, glm::vec3& outWorldMax) {
		glm::vec3 localMin, localMax;
		mesh->getLocalAABB(localMin, localMax);

		// Las 8 esquinas de la caja en espacio local
		glm::vec3 corners[8] = {
			glm::vec3(localMin.x, localMin.y, localMin.z),
			glm::vec3(localMax.x, localMin.y, localMin.z),
			glm::vec3(localMin.x, localMax.y, localMin.z),
			glm::vec3(localMax.x, localMax.y, localMin.z),
			glm::vec3(localMin.x, localMin.y, localMax.z),
			glm::vec3(localMax.x, localMin.y, localMax.z),
			glm::vec3(localMin.x, localMax.y, localMax.z),
			glm::vec3(localMax.x, localMax.y, localMax.z)
		};

		// Obtenemos la matriz de modelo de nuestro componente Transform
		glm::mat4 modelMatrix = transform->getModelMatrix();

		// Inicializamos con valores extremos inversos
		outWorldMin = glm::vec3(std::numeric_limits<float>::max());
		outWorldMax = glm::vec3(std::numeric_limits<float>::lowest());

		// Transformamos cada esquina y actualizamos los nuevos min y max globales
		for (int i = 0; i < 8; ++i) {
			glm::vec4 worldPos = modelMatrix * glm::vec4(corners[i], 1.0f);

			outWorldMin.x = std::min(outWorldMin.x, worldPos.x);
			outWorldMin.y = std::min(outWorldMin.y, worldPos.y);
			outWorldMin.z = std::min(outWorldMin.z, worldPos.z);

			outWorldMax.x = std::max(outWorldMax.x, worldPos.x);
			outWorldMax.y = std::max(outWorldMax.y, worldPos.y);
			outWorldMax.z = std::max(outWorldMax.z, worldPos.z);
		}
	}

	std::weak_ptr<ec::Entity> Raycast::castRay() {
		glm::vec4 viewPort(0, 0, gla().viewportWidth(), gla().viewportHeight());
		Camera* cam = sceneM().activeScene()->getCamera();

		glm::mat4 view = cam->getViewMat();
		glm::mat4 projection = cam->getProjectionMat();

		glm::vec3 nearPoint = glm::unProject(
			glm::vec3(inpM().getViewportMouseX(), gla().viewportHeight() - inpM().getViewportMouseY(), 0.0f),
			view,
			projection,
			viewPort
		);

		glm::vec3 farPoint = glm::unProject(
			glm::vec3(inpM().getViewportMouseX(), gla().viewportHeight() - inpM().getViewportMouseY(), 1.0f),
			view,
			projection,
			viewPort
		);

		// Rayo del cast
		_rayDirection = glm::normalize(farPoint - nearPoint);
		_rayOrigin = nearPoint;

		ec::entity_t selectedGO;
		float nearestGO = std::numeric_limits<float>::max();
		for (const auto& group : sceneM().activeScene()->getSceneObjects()) {
			for (const auto& gO : group) {
				Transform* tr = gO->getComponent<Transform>();
				MeshRenderer* mr = gO->getComponent<MeshRenderer>();

				if (tr && mr) {
					glm::vec3 boxMin, boxMax;
					calculateWorldAABB(mr, tr, boxMin, boxMax);

					float distance = 0;
					bool intersects = rayIntersectsWithObj(boxMin, boxMax, distance);
					if (intersects && distance < nearestGO) {
						selectedGO = gO;
						nearestGO = distance;
					}
				}
			}
		}

		return selectedGO;
	}
}