#include "InspectorWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <component/Transform.h>

namespace capiEngine::ui {
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

			ImGui::SeparatorText("Components");
			if (ImGui::CollapsingHeader("Transform")) {
				glm::vec3& pos = _selectedEnt->getComponent<Transform>()->getPosition();
				glm::vec3& scl = _selectedEnt->getComponent<Transform>()->getScale();
				glm::vec3& rot = _selectedEnt->getComponent<Transform>()->getRotation();

				if (ImGui::BeginTable("TransformTable", 4)) {
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Position");

					ImGui::TableSetColumnIndex(1);
					ImGui::InputFloat("X###px", &pos.x, 1, 10);
					ImGui::TableSetColumnIndex(2);
					ImGui::InputFloat("Y###py", &pos.y, 1, 10);
					ImGui::TableSetColumnIndex(3);
					ImGui::InputFloat("Z###pz", &pos.z, 1, 10);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Scale");
					ImGui::TableSetColumnIndex(1);
					ImGui::InputFloat("X###sx", &scl.x, 1, 10);
					ImGui::TableSetColumnIndex(2);
					ImGui::InputFloat("Y###sy", &scl.y, 1, 10);
					ImGui::TableSetColumnIndex(3);
					ImGui::InputFloat("Z###sz", &scl.z, 1, 10);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Rotation");
					ImGui::TableSetColumnIndex(1);
					ImGui::InputFloat("X###rx", &rot.x, 1, 10);
					ImGui::TableSetColumnIndex(2);
					ImGui::InputFloat("Y###ry", &rot.y, 1, 10);
					ImGui::TableSetColumnIndex(3);
					ImGui::InputFloat("Z###rz", &rot.z, 1, 10);

					ImGui::EndTable();
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