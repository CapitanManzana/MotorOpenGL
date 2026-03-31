#include "InspectorWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/GLApplication.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <utils/AssimpLoader.h>
#include <managers/ResourceManager.h>
#include <core/Mesh.h>

namespace cme::ui {
	void InspectorWindow::renderWindowContent() {
		static char importPath[256] = "assets/models/model.glb";
		ImGui::SeparatorText("Import Asset");
		ImGui::InputText("Path", importPath, sizeof(importPath));
		if (ImGui::Button("Import file")) {
			auto meshes = AssimpLoader::loadModel(importPath);
			if (!meshes.empty()) {
				for (size_t i = 0; i < meshes.size(); ++i) {
					std::string name = std::filesystem::path(importPath).stem().string();
					if (meshes.size() > 1) name += "_" + std::to_string(i);
					
					rscrM().registerMesh(name, meshes[i]);
					
					auto entity = sceneM().activeScene()->addGameObject(sceneM().activeScene(), name);
					entity->addComponent<Transform>();
					auto renderer = entity->addComponent<MeshRenderer>();
					
					meshes[i]->setShader(rscrM().getShader("default"));
					renderer->setMesh(meshes[i].get());
				}
			}
		}
		ImGui::Dummy(ImVec2(0, 10));

		if (auto entitySp = _selectedEnt.lock()) {
			ImGui::SeparatorText("GameObject");

			if (ImGui::Button("Delete Entity")) {
				entitySp->die();
				_selectedEnt.reset();
			}
			ImGui::SameLine();
			ImGui::Checkbox("Active", &entitySp->active());
			ImGui::SameLine(0, 10);

			char buffer[256] = "";
			strncpy_s(buffer, sizeof(buffer), entitySp->name().c_str(), _TRUNCATE);
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
				auto& name = entitySp->name();
				name = buffer;
			}

			for (auto& comp : entitySp->_components) {
				if (comp) {
					if (comp->serializeID() == "Transform") {
						auto tr = dynamic_cast<Transform*>(comp);
						if (tr) {
							if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
								glm::vec3 pos = tr->getPosition();
								if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) tr->setPosition(pos);

								glm::vec3 rot = tr->getRotation();
								if (ImGui::DragFloat3("Rotation", &rot.x, 0.5f)) tr->setRotation(rot);

								glm::vec3 scale = tr->getScale();
								if (ImGui::DragFloat3("Scale", &scale.x, 0.05f)) tr->setScale(scale);
							}
							ImGui::Dummy(ImVec2(0, 5));
							continue;
						}
					}
					
					comp->drawOnInspector();
					ImGui::Dummy(ImVec2(0, 5));
				}
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