#include "SceneWindow.h"
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <utils/logger.h>
#include <format>

namespace cme::ui {
	void SceneWindow::renderWindowContent() {
		Scene* scene = sceneM().activeScene();
		int groupId = 0;
		int entId = 0;

		if (ImGui::CollapsingHeader(("Scene hierarchy: " + scene->name()).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			for (auto& goGroup : scene->getSceneObjects()) {
				entId = 0;
				for (auto& go : goGroup) {
					ImGui::PushID(entId);

					if (ImGui::Selectable(go->name().c_str()))
						if (_selectedEntCallback) _selectedEntCallback(go);

					ImGui::PopID();
					entId++;
				}

				groupId++;
			}
		}
	}
}