#include "Light.h"
#include <imgui.h>
#include <ec/entity.h>
#include <component/Transform.h>
#include <core/serialize/JsonSerializer.h>
#include <component/MeshRenderer.h>
#include <managers/LightManager.h>

namespace cme {
    Light::~Light() {
        if (_lightIndex >= 0) {
            lightM().removeLight(_lightIndex);
            _lightIndex = -1;
        }
    }

    void Light::initComponent() {
        if (auto entitySp = _entity.lock()) {
            _tr = entitySp->getComponent<Transform>();
            assert(_tr != nullptr && "El transform de un Light es null");

            // Sincronizamos la posicion inicial antes de registrar
            _pointLight.position = _tr->getPosition();

            // Registramos la luz en el manager y guardamos el indice
            _lightIndex = lightM().addLight(_pointLight);

            // Si la entidad tiene mesh, la marcamos como fuente de luz visual
            if (auto mesh = entitySp->getComponent<MeshRenderer>())
                mesh->setLightSource(true);
        }
    }

    void Light::update() {
        // Mantenemos la posicion sincronizada con el Transform en cada frame
        if (_lightIndex >= 0) {
            _pointLight.position = _tr->getPosition();
            lightM().setLight(_lightIndex, _pointLight);
        }
    }

    void Light::drawOnInspector() {
        if (ImGui::CollapsingHeader("Light")) {
            bool changed = false;

            changed |= ImGui::ColorEdit3("Color", glm::value_ptr(_pointLight.color));
            changed |= ImGui::DragFloat("Intensity", &_pointLight.intensity, 0.01f, 0.0f, 100.0f);

            if (ImGui::TreeNode("Atenuacion")) {
                changed |= ImGui::DragFloat("Constant", &_pointLight.constant, 0.001f, 0.0f, 10.0f);
                changed |= ImGui::DragFloat("Linear", &_pointLight.linear, 0.001f, 0.0f, 10.0f);
                changed |= ImGui::DragFloat("Quadratic", &_pointLight.quadratic, 0.001f, 0.0f, 10.0f);
                ImGui::TreePop();
            }

            // Actualizamos el manager solo si algo cambio en el inspector
            if (changed && _lightIndex >= 0)
                lightM().setLight(_lightIndex, _pointLight);
        }
    }

    void Light::serialize(JsonSerializer& s) const {
        s.write("color", _pointLight.color);
        s.write("intensity", _pointLight.intensity);
        s.write("constant", _pointLight.constant);
        s.write("linear", _pointLight.linear);
        s.write("quadratic", _pointLight.quadratic);
    }

    void Light::deserialize(JsonSerializer& s) {
        _pointLight.color = s.readVec3("color");
        _pointLight.intensity = s.readFloat("intensity");
        _pointLight.constant = s.readFloat("constant");
        _pointLight.linear = s.readFloat("linear");
        _pointLight.quadratic = s.readFloat("quadratic");
        // La posicion se sincroniza en initComponent/update, no se serializa
    }
}