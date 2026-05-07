#include "windows/InspectorWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <component/Light.h>

#include <drawGUI/material/MaterialGUI.h>
#include <drawGUI/component/TransformGUI.h>
#include <drawGUI/component/MeshRendererGUI.h>
#include <drawGUI/component/LightGUI.h>

namespace cme::editor {
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

			ImGui::SameLine();

			if (ImGui::Button("Erase")) {
				entitySp->setIsAlive(false);
			}

			ImGui::Dummy(ImVec2(0, 10));
			ImGui::SeparatorText("Components");
			int i = 0;

			std::vector<ec::Component*> compToErase;
			for (auto& comp : entitySp->_components) {
				if (comp) {
					// Obtenemos el tipo real del componente
					std::type_index type(typeid(*comp));

					// Buscamos si el Editor sabe cómo dibujar este tipo
					auto it = _componentUIRegistry.find(type);
					if (it != _componentUIRegistry.end()) {

						if (comp->getID() != ec::comp::cmpID::TRANSFORM) {
							ImGui::PushID(i);
							if (ImGui::Button("Erase")) {
								compToErase.push_back(comp);
							}
							ImGui::PopID();
							ImGui::SameLine();
						}
						
						it->second(comp);
						ImGui::Dummy(ImVec2(0, 5));
					}

					i++;
				}
			}

			for (auto& comp : compToErase) {
				entitySp->removeComponent(comp->getID());
			}
			compToErase.clear();

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

			ImGui::Separator();
			if (auto m = entitySp->getComponent<MeshRenderer>()) {
				MaterialGUI mat(m->material());
				mat.drawOnInspector();
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

	void InspectorWindow::registerUIComponents() {
		// Registramos el Transform
		_componentUIRegistry[typeid(cme::Transform)] = [](ec::Component* c) {
			TransformGUI gui(static_cast<Transform*>(c));
			gui.drawOnInspector();
			};

		// Registramos el MeshRenderer
		_componentUIRegistry[typeid(cme::MeshRenderer)] = [](ec::Component* c) {
			MeshRendererGUI gui(static_cast<MeshRenderer*>(c));
			gui.drawOnInspector();
			};

		_componentUIRegistry[typeid(cme::Light)] = [](ec::Component* c) {
			LightGUI gui(static_cast<Light*>(c));
			gui.drawOnInspector();
		};
	}
}