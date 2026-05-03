#include <drawGUI/component/GlobalLightGUI.h>
#include <lighting/GlobalLight.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace cme::editor {
	GlobalLightGUI::GlobalLightGUI(GlobalLight* globalLight) : _globalLight(globalLight) {
	}

	void GlobalLightGUI::drawOnInspector() {
		ImGui::Text("Light Color");
		ImGui::SameLine(0.0f, 10.0f);
		ImGui::ColorEdit3("##Light Color", glm::value_ptr(_globalLight->_color));

        bool changed = false;
        changed |= ImGui::SliderFloat("Azimuth", &_globalLight->_azimuth, -180.0f, 180.0f, "%.1f deg");
        changed |= ImGui::SliderFloat("Elevation", &_globalLight->_elevation, -90.0f, 90.0f, "%.1f deg");

        if (changed) {
            float az = glm::radians(_globalLight->_azimuth);
            float el = glm::radians(_globalLight->_elevation);
            _globalLight->_dir = glm::normalize(glm::vec3(
                cos(el) * sin(az),
                sin(el),
                cos(el) * cos(az)
            ));
        }

		ImGui::Text("Light Intensity");
		ImGui::SameLine();
		ImGui::DragFloat("##Light Intensity", &_globalLight->_intensity, 0.0f);
	}
}
