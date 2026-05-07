#include <drawGUI/component/MeshRendererGUI.h>
#include <component/MeshRenderer.h>
#include <mesh/Mesh.h>
#include <mesh/CubeMesh.h>
#include <mesh/TriangleMesh.h>
#include <mesh/QuadMesh.h>
#include <imgui.h>

namespace cme::editor {
	MeshRendererGUI::MeshRendererGUI(MeshRenderer* meshRenderer) : _meshRenderer(meshRenderer) {}

	void MeshRendererGUI::drawOnInspector() {
		if (ImGui::CollapsingHeader("Mesh Renderer")) {
			if (ImGui::BeginTable("MeshRendTable", 2)) {
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Mesh Type");
				ImGui::TableSetColumnIndex(1);

				if (ImGui::BeginCombo("##combo", _meshRenderer->_currentMeshType.c_str())) {
					for (auto& opcion : _meshRenderer->MESH_T_NAMES) {
						bool isSelected = _meshRenderer->_currentMeshType == opcion;
						if (ImGui::Selectable(opcion, isSelected)) {
							_meshRenderer->_currentMeshType = opcion;

							_meshRenderer->_mesh.release();
							if (_meshRenderer->_currentMeshType == _meshRenderer->MESH_T_NAMES[1]) {
								_meshRenderer->_mesh = std::make_unique<TriangleMesh>();
							}
							else if (_meshRenderer->_currentMeshType == _meshRenderer->MESH_T_NAMES[2]) {
								_meshRenderer->_mesh = std::make_unique<QuadMesh>();
							}
							else if (_meshRenderer->_currentMeshType == _meshRenderer->MESH_T_NAMES[3]) {
								_meshRenderer->_mesh = std::make_unique<CubeMesh>();
							}
							else {
								_meshRenderer->_mesh = nullptr;
							}
						}
						if (isSelected && ImGui::IsWindowAppearing()) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				ImGui::EndTable();
			}
		}
	}
}

