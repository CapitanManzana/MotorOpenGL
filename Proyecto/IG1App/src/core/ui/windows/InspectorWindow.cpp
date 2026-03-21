#include "InspectorWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>

namespace capiEngine::ui {
	void InspectorWindow::renderWindowContent() const {
		Camera* cam = sceneM().activeScene()->getCamera();

		ImGui::Text("Delta Time: %.3f", gla().deltaTime());
		ImGui::Text("FPS: %.1f", 1.0f / gla().deltaTime());
		glm::vec3 pos = cam->getPosition();
		float pitch = cam->getPitch();
		float yaw = cam->getYaw();
		ImGui::Text("Camera position: X: %.2f  Y: %.2f  Z: %.2f  (yaw: %.2f  pitch: %0.2f)", pos.x, pos.y, pos.z, yaw, pitch);
	}
}