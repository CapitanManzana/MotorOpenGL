#include "InspectorWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <component/Light.h>

namespace cme::ui {
	void InspectorWindow::renderWindowContent() {
		if (auto entitySp = _selectedEnt.lock()) {
			ImGui::SeparatorText("GameObject");

			ImGui::Checkbox("Active", &entitySp->active());
			ImGui::SameLine(0, 10);

			char buffer[256] = "";
			strncpy_s(buffer, sizeof(buffer), entitySp->name().c_str(), _TRUNCATE);
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
				auto& name = entitySp->name();
				name = buffer;
			}

			ImGui::Dummy(ImVec2(0, 10));
			ImGui::SeparatorText("Components");
			for (auto& comp : entitySp->_components) {
				if (comp) {
					comp->drawOnInspector();
					ImGui::Dummy(ImVec2(0, 5));
				}
			}

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("add_component_popup");
			}

			if (ImGui::BeginPopup("add_component_popup")) {
				ImGui::TextDisabled("Componentes");
				ImGui::Separator();

				if (ImGui::Selectable("Transform") && !entitySp->hasComponent<Transform>())	
					entitySp->addComponent<Transform>();
				if (ImGui::Selectable("Mesh Renderer") && !entitySp->hasComponent<MeshRenderer>())
					entitySp->addComponent<MeshRenderer>();
				if (ImGui::Selectable("Light") && !entitySp->hasComponent<Light>())
					entitySp->addComponent<Light>();

				ImGui::EndPopup();
			}
		}
		else {
			_selectedEnt.reset();
		}

		ImGui::Dummy(ImVec2(0, 30));

		Camera* cam = sceneM().activeScene()->getCamera();
		ImGui::Text("Delta Time: %.3f", gla().deltaTime());
		ImGui::Text("FPS: %.1f", 1.0f / gla().deltaTime());

		glm::vec3 pos = cam->getPosition();
		float pitch = cam->getPitch();
		float yaw = cam->getYaw();
		ImGui::Text("Camera position: X: %.2f  Y: %.2f  Z: %.2f  (yaw: %.2f  pitch: %0.2f)", pos.x, pos.y, pos.z, yaw, pitch);
	}
}