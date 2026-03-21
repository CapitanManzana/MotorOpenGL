#include "InspectorWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>

namespace capiEngine::ui {
	void InspectorWindow::renderWindowContent() const {
		ImGui::Text("Delta Time: %.3f", gla().deltaTime());
		glm::vec3 pos = sceneM().activeScene()->getCamera()->getPosition();
		ImGui::Text("Camera position: X: %.2f  Y: %.2f  Z: %.2f", pos.x, pos.y, pos.z);
	}
}