#pragma once
#include <imgui.h>
#include <string>
#include <glm/vec3.hpp>

namespace cme::ui {
    inline void vec3Row(const char* label, glm::vec3& v, const char* id) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text(label);
        ImGui::TableSetColumnIndex(1); ImGui::InputFloat(("X##" + std::string(id) + "x").c_str(), &v.x, 1, 10);
        ImGui::TableSetColumnIndex(2); ImGui::InputFloat(("Y##" + std::string(id) + "y").c_str(), &v.y, 1, 10);
        ImGui::TableSetColumnIndex(3); ImGui::InputFloat(("Z##" + std::string(id) + "z").c_str(), &v.z, 1, 10);
    }
}