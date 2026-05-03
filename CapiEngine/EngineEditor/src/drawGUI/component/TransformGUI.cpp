#include <drawGUI/component/TransformGUI.h>
#include <component/Transform.h>
#include <utils/ImGuiUtils.h>
#include <imgui.h>

namespace cme::editor {
	TransformGUI::TransformGUI(Transform* transform) : _transform(transform) {
	}

	void TransformGUI::drawOnInspector() {
		if (ImGui::CollapsingHeader("Transform")) {
			if (ImGui::BeginTable("TransformTable", 4)) {
				cme::ui::vec3Row("Position", _transform->getPosition(), "p");
				cme::ui::vec3Row("Scale", _transform->getScale(), "s");
				cme::ui::vec3Row("Rotation", _transform->getRotation(), "r");
				ImGui::EndTable();
			}
		}
	}
}
