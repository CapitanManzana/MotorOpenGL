#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ec/entity.h>

namespace cme {
	class MeshRenderer;
	class Transform;

	class Raycast
	{
	private:
		glm::vec3 _rayDirection;
		glm::vec3 _rayOrigin;
	public:
		Raycast() = default;

		std::weak_ptr<ec::Entity> castRay();
	private:
		bool rayIntersectsWithObj(glm::vec3 boxMin, glm::vec3 boxMax, float& collisionDist);
		void calculateWorldAABB(MeshRenderer* mesh, Transform* transform, glm::vec3& outWorldMin, glm::vec3& outWorldMax);
	};
}


