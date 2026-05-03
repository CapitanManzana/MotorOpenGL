#include <drawGUI/material/MaterialGUI.h>
#include <surface/Material.h>
#include <managers/ResourceManager.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace cme::editor {
	MaterialGUI::MaterialGUI(Material* material) : _material(material) {
	}

	void MaterialGUI::drawOnInspector() {
		if (!ImGui::CollapsingHeader("Material")) return;

		// Combo de shaders
		if (ImGui::BeginCombo("Shader", _material->_shader ? _material->_shader->getName().c_str() : "None")) {
			for (auto& name : rscrM().getAllShaderNames()) {
				if (ImGui::Selectable(name.c_str())) {
					_material->setShader(rscrM().getShader(name));
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();

		// Propiedades dinámicas según el tipo
		for (auto& [name, prop] : _material->_properties) {
			std::visit([&](auto&& val) {
				using T = std::decay_t<decltype(val)>;

				if constexpr (std::is_same_v<T, float>)
					ImGui::SliderFloat(name.c_str(), &std::get<float>(prop.value), 0.f, 256.f);
				else if constexpr (std::is_same_v<T, glm::vec3>)
					ImGui::ColorEdit3(name.c_str(), glm::value_ptr(std::get<glm::vec3>(prop.value)));
				else if constexpr (std::is_same_v<T, glm::vec4>)
					ImGui::ColorEdit4(name.c_str(), glm::value_ptr(std::get<glm::vec4>(prop.value)));
				else if constexpr (std::is_same_v<T, bool>)
					ImGui::Checkbox(name.c_str(), &std::get<bool>(prop.value));
				}, prop.value);
		}

		ImGui::Separator();
		ImGui::Text("Texturas");
		ImGui::Dummy(ImVec2(0, 10));
		for (auto& [name, tex] : _material->_textures) {
			// Combo de texturas disponibles
			std::string label = tex ? tex->name() : "None";
			if (ImGui::BeginCombo(name.c_str(), label.c_str())) {
				// Opción para quitar textura
				if (ImGui::Selectable("None"))
					tex = nullptr;

				for (auto& texName : rscrM().getAllTextureNames()) {
					bool sel = (tex && tex->name() == texName);
					if (ImGui::Selectable(texName.c_str(), sel))
						tex = rscrM().getTexture(texName);
					if (sel) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();

			// Preview
			if (tex)
				ImGui::Image((ImTextureID)(intptr_t)tex->id(), ImVec2(48, 48));
			else
				ImGui::Dummy(ImVec2(48, 48));
		}
	}
}
