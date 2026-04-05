#include "Light.h"
#include <imgui.h>
#include <ec/entity.h>
#include <component/Transform.h>
#include <core/serialize/JsonSerializer.h>
#include <component/MeshRenderer.h>

namespace cme {
	std::vector<Light*> Light::_allLights;

	Light::~Light() {
		auto it = std::find(_allLights.begin(), _allLights.end(), this);
		if (it != _allLights.end())
			_allLights.erase(it);
	}

	void Light::initComponent() {
		if (auto entitySp = _entity.lock()) {
			_tr = entitySp->getComponent<Transform>();
			auto mesh = entitySp->getComponent<MeshRenderer>();

			assert(_tr != nullptr && "El transform de un light source es null");

			if (mesh) mesh->setLightSource(true);

			_allLights.push_back(this);
		}
	}

	void Light::drawOnInspector() {
		if (ImGui::CollapsingHeader("Light")) {
			ImGui::ColorEdit3("Light Color", glm::value_ptr(_color));
			ImGui::InputFloat("Light Intensity", &_intensity);
			ImGui::InputFloat("Ambient Strength", &_ambienteStrength);
		}
	}

	void Light::serialize(JsonSerializer& s) const {
		s.write("color", _color);
		s.write("intensity", _intensity);
		s.write("ambientStrength", _ambienteStrength);
	}

	void Light::deserialize(JsonSerializer& s) {
		_color = s.readVec4("color");
		_intensity = s.readFloat("intensity");
		_ambienteStrength = s.readFloat("ambientStrength");
	}
}