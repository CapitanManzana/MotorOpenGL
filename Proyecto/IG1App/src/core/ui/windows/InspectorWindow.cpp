#include "InspectorWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <component/Transform.h>

namespace cme::ui {
	void InspectorWindow::renderWindowContent() {
		if (_selectedEnt) {
			ImGui::SeparatorText("GameObject");

			ImGui::Checkbox("Active", &_selectedEnt->active());
			ImGui::SameLine(0, 10);

			char buffer[256] = "";
			strncpy_s(buffer, sizeof(buffer), _selectedEnt->name().c_str(), _TRUNCATE);
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
				auto& name = _selectedEnt->name();
				name = buffer;
			}

			ImGui::Dummy(ImVec2(0, 10));
			ImGui::SeparatorText("Components");
			for (auto& comp : _selectedEnt->_components) {
				if (comp) {
					comp->drawOnInspector();
					ImGui::Dummy(ImVec2(0, 5));
				}
			}
		}

		ImGui::Dummy(ImVec2(0, 30));

		Camera* cam = sceneM().activeScene()->getCamera();
		ImGui::Text("Delta Time: %.3f", gla().deltaTime());
		ImGui::Text("FPS: %.1f", 1.0f / gla().deltaTime());

		if (_selectedEnt) ImGui::Text("Entidad Seleccionada: %s", _selectedEnt->name().c_str());
		else ImGui::Text("Entidad Seleccionada: NONE");

		glm::vec3 pos = cam->getPosition();
		float pitch = cam->getPitch();
		float yaw = cam->getYaw();
		ImGui::Text("Camera position: X: %.2f  Y: %.2f  Z: %.2f  (yaw: %.2f  pitch: %0.2f)", pos.x, pos.y, pos.z, yaw, pitch);
	}
}