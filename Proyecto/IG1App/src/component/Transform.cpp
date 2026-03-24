#include "Transform.h"
#include <core/serialize/JsonSerializer.h>
#include <imgui.h>

namespace cme {
    glm::mat4 Transform::getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        // Primero trasladar
        model = glm::translate(model, _position);
        // Luego rotar sobre su propio eje
        model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
        // Por último escalar
        model = glm::scale(model, _scale);
        return model;
    }

    void Transform::serialize(JsonSerializer& s) const {
        s.write("position", _position);
        s.write("scale", _scale);
        s.write("rotation", _rotation);
    }

    void Transform::deserialize(JsonSerializer& s) {
        _position = s.readVec3("position");
        _scale = s.readVec3("scale");
        _rotation = s.readVec3("rotation");
    }

    void Transform::drawOnInspector() {
		if (ImGui::CollapsingHeader("Transform")) {
			if (ImGui::BeginTable("TransformTable", 4)) {
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Position");

				ImGui::TableSetColumnIndex(1);
				ImGui::InputFloat("X###px", &_position.x, 1, 10);
				ImGui::TableSetColumnIndex(2);
				ImGui::InputFloat("Y###py", &_position.y, 1, 10);
				ImGui::TableSetColumnIndex(3);
				ImGui::InputFloat("Z###pz", &_position.z, 1, 10);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Scale");
				ImGui::TableSetColumnIndex(1);
				ImGui::InputFloat("X###sx", &_scale.x, 1, 10);
				ImGui::TableSetColumnIndex(2);
				ImGui::InputFloat("Y###sy", &_scale.y, 1, 10);
				ImGui::TableSetColumnIndex(3);
				ImGui::InputFloat("Z###sz", &_scale.z, 1, 10);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Rotation");
				ImGui::TableSetColumnIndex(1);
				ImGui::InputFloat("X###rx", &_rotation.x, 1, 10);
				ImGui::TableSetColumnIndex(2);
				ImGui::InputFloat("Y###ry", &_rotation.y, 1, 10);
				ImGui::TableSetColumnIndex(3);
				ImGui::InputFloat("Z###rz", &_rotation.z, 1, 10);

				ImGui::EndTable();
			}
		}
    }
}