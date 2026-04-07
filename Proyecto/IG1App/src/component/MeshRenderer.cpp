#include "MeshRenderer.h"
#include <imgui.h>

#include <ec/entity.h>

#include <core/Mesh.h>
#include <core/Camera.h>
#include <core/Scene.h>

#include <component/Transform.h>
#include <core/serialize/JsonSerializer.h>
#include <core/mesh/CubeMesh.h>
#include <core/mesh/TriangleMesh.h>
#include <core/mesh/QuadMesh.h>
#include <managers/ResourceManager.h>

namespace cme {
	MeshRenderer::MeshRenderer(Mesh* mesh) : _mesh(mesh) {
		_mat = new Material();
		_mesh->setMaterial(_mat);
	}

	MeshRenderer::~MeshRenderer() {
		delete _mesh; // Esto invocará Mesh::~Mesh() y liberará VBOs/VAOs y memoria
	}

	void MeshRenderer::render() const {
		if (!_mesh) return;
		_mesh->setModelMatrix(_tr->getModelMatrix());

		if (auto ent = _entity.lock()) {
			_cam->uploadToGPU(_mesh, ent);
		}
		_mesh->render();
	}

	void MeshRenderer::initComponent() {
		if (auto entitySp = _entity.lock()) {
			_cam = entitySp->getScene()->getCamera();
			_tr = entitySp->getComponent<Transform>();

			assert(_tr != nullptr);

			if (_mesh) {
				int meshID = _mesh->id();
				if (meshID >= 0 && meshID < MESH_T_NAMES.size()) _currentMeshType = MESH_T_NAMES[meshID];
				else LOG_ERROR(std::format("El id del mesh no es valido. Entidad: {} | ID: ", entitySp->name(), meshID));
			}
		}
	}

	void MeshRenderer::setLightSource(bool value) {
		_mesh->setLightSource(value);
	}

	glm::mat3& MeshRenderer::normalMatrix() {
		return _mesh->normalMatrix(); 
	}

	Material* MeshRenderer::material() {
		return _mesh->material();
	}

	void MeshRenderer::getLocalAABB(glm::vec3& outMin, glm::vec3& outMax) const {
		_mesh->getLocalAABB(outMin, outMax);
	}

	void MeshRenderer::serialize(JsonSerializer& s) const {
		s.write("mesh", (int)_mesh->id());
		s.write("isLight", _mesh->isLightSource());
	}

	void MeshRenderer::deserialize(JsonSerializer& s) {
		int meshID = s.readInt("mesh");
		bool isLight = s.readInt("isLight");

		if (meshID == 1)      _mesh = new TriangleMesh();
		else if (meshID == 2) _mesh = new QuadMesh();
		else if (meshID == 3) _mesh = new CubeMesh();

		if (!_mesh) {
			LOG_ERROR("La mesh es nula despues de cargarla del archivo");
			return;
		}

		// Solo si es luz, si no el shader ya está bien puesto en el constructor
		if (isLight) _mesh->setLightSource(true);

		if (auto entitySp = _entity.lock()) {
			if (meshID >= 0 && meshID < MESH_T_NAMES.size())
				_currentMeshType = MESH_T_NAMES[meshID];
		}
	}

	void MeshRenderer::drawOnInspector() {
		if (auto entitySp = _entity.lock()) {
			if (ImGui::CollapsingHeader("Mesh Renderer")) {
				if (ImGui::BeginTable("MeshRendTable", 2)) {
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Mesh Type");
					ImGui::TableSetColumnIndex(1);

					if (ImGui::BeginCombo("##combo", _currentMeshType.c_str())) {
						for (auto& opcion : MESH_T_NAMES) {
							bool isSelected = _currentMeshType == opcion;
							if (ImGui::Selectable(opcion, isSelected)) {
								_currentMeshType = opcion;

								delete _mesh;
								if (_currentMeshType == MESH_T_NAMES[1]) {
									_mesh = new TriangleMesh();
								}
								else if (_currentMeshType == MESH_T_NAMES[2]) {
									_mesh = new QuadMesh();
								}
								else if (_currentMeshType == MESH_T_NAMES[3]) {
									_mesh = new CubeMesh();
								}
								else {
									_mesh = nullptr;
								}
							}
							if (isSelected && ImGui::IsWindowAppearing()) {
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Shader");
					ImGui::TableSetColumnIndex(1);

					/*if (ImGui::BeginCombo("##comboShader", _shaderName.c_str())) {
						for (auto& opcion : rscrM().getAllShaderNames()) {
							bool isSelected = _shaderName == opcion;
							if (ImGui::Selectable(opcion.c_str(), isSelected)) {
								_shaderName = opcion;
								Shader* shader = rscrM().getShader(opcion);
								if (shader) {
									if (_mesh) _mesh->setShader(shader);
									_shaderName = opcion;
								}
								else {
									LOG_WARN(std::format("EL shader seleccionado para el mesh renderer es inexistente. Entity: {} | Shader: {}", entitySp->name(), opcion));
								}
							}
							if (isSelected && ImGui::IsWindowAppearing()) {
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}*/

					ImGui::EndTable();
				}
			}
		}
	}
}