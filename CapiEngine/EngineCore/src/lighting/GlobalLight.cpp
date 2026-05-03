#include "GlobalLight.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <utils/ImGuiUtils.h>
#include <core/serialize/JsonSerializer.h>

namespace cme {
	void GlobalLight::drawOnInspector() {
		ImGui::Text("Light Color");
		ImGui::SameLine(0.0f, 10.0f);
		ImGui::ColorEdit3("##Light Color", glm::value_ptr(_color));

        bool changed = false;
        changed |= ImGui::SliderFloat("Azimuth", &_azimuth, -180.0f, 180.0f, "%.1f deg");
        changed |= ImGui::SliderFloat("Elevation", &_elevation, -90.0f, 90.0f, "%.1f deg");

        if (changed) {
            float az = glm::radians(_azimuth);
            float el = glm::radians(_elevation);
            _dir = glm::normalize(glm::vec3(
                cos(el) * sin(az),
                sin(el),
                cos(el) * cos(az)
            ));
        }

		ImGui::Text("Light Intensity");
		ImGui::SameLine();
		ImGui::DragFloat("##Light Intensity", &_intensity, 0.0f);
	}

    void GlobalLight::serialize(JsonSerializer& s) const {
        s.write("lightColor", _color);
        s.write("lightDir", _dir);
        s.write("lightIntensity", _intensity);
    }

    void GlobalLight::deserialize(JsonSerializer& s) {
        _color = s.readVec3("lightColor");
        _dir = s.readVec3("lightDir");
        _intensity = s.readFloat("lightIntensity");

        _elevation = asin(_dir.y);
        _azimuth = asin(_dir.x / cos(_elevation));
    }
}