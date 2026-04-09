#include "GlobalLight.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <utils/ImGuiUtils.h>

namespace cme {
	void GlobalLight::drawOnInspector() {
		ImGui::Text("Light Color");
		ImGui::SameLine(0.0f, 10.0f);
		ImGui::ColorEdit3("##Light Color", glm::value_ptr(_color));

		if (ImGui::BeginTable("LightDir", 4)) {
			ui::vec3Row("Light Direction", _dir, "ld");
			ImGui::EndTable();
		}
		ImGui::Text("Light Intensity");
		ImGui::SameLine();
		ImGui::DragFloat("##Light Intensity", &_intensity, 0.0f);
	}
}