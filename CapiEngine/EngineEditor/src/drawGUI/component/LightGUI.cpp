#include <drawGUI/component/LightGUI.h>
#include <component/Light.h>
#include <managers/LightManager.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace cme::editor {
	LightGUI::LightGUI(Light* light) : _light(light) {
	}

	void LightGUI::drawOnInspector() {
		if (ImGui::CollapsingHeader("Light")) {
			bool changed = false;

			changed |= ImGui::ColorEdit3("Color", glm::value_ptr(_light->_pointLight.color));
			changed |= ImGui::DragFloat("Intensity", &_light->_pointLight.intensity, 0.01f, 0.0f, 100.0f);

			if (ImGui::TreeNode("Atenuacion")) {
				changed |= ImGui::DragFloat("Constant", &_light->_pointLight.constant, 0.001f, 0.0f, 10.0f);
				changed |= ImGui::DragFloat("Linear", &_light->_pointLight.linear, 0.001f, 0.0f, 10.0f);
				changed |= ImGui::DragFloat("Quadratic", &_light->_pointLight.quadratic, 0.001f, 0.0f, 10.0f);
				ImGui::TreePop();
			}

			// Actualizamos el manager solo si algo cambio en el inspector
			if (changed && _light->_lightIndex >= 0)
				lightM().setLight(_light->_lightIndex, _light->_pointLight);
		}
	}
}
