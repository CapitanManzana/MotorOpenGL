#include "MeshRenderer.h"
#include <imgui.h>
#include <ec/entity.h>
#include <core/Mesh.h>
#include <core/Camera.h>
#include <core/Scene.h>
#include <core/Shader.h>
#include <component/Transform.h>
#include <managers/ResourceManager.h>
#include <managers/SceneManager.h>

namespace cme {
	MeshRenderer::~MeshRenderer() {}

	void MeshRenderer::initComponent() {
		_tr = _entity.lock()->getComponent<Transform>();
	}

	void MeshRenderer::getLocalAABB(glm::vec3& outMin, glm::vec3& outMax) const {
		if (_mesh) {
			_mesh->getLocalAABB(outMin, outMax);
		}
		else {
			outMin = outMax = glm::vec3(0.0f);
		}
	}

	void MeshRenderer::render() const {
		if (_mesh && _tr) {
			Shader* shader = nullptr;
			if (_shaderName != "") {
				shader = rscrM().getShader(_shaderName);
			}
			else {
				shader = _mesh->shader();
			}

			Camera* cam = sceneM().activeScene()->getCamera();
			if (shader && cam) {
				shader->use();
				
				glm::mat4 view = cam->getViewMat();
				glm::mat4 proj = cam->getProjectionMat();
				glm::mat4 model = _tr->getModelMatrix();

				shader->setUniform("modelView", view * model);
				shader->setUniform("projection", proj);
				shader->setUniform("model", model);
				
				_mesh->render();
			}
		}
	}

	void MeshRenderer::drawOnInspector() {
		if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("Mesh Type: %s", _currentMeshType.c_str());
			if (_mesh) {
				ImGui::Text("Vertices: %d", (int)_mesh->getNumVertices());
			}

			char shaderBuf[64];
			strncpy_s(shaderBuf, sizeof(shaderBuf), _shaderName.c_str(), _TRUNCATE);
			if (ImGui::InputText("Shader", shaderBuf, sizeof(shaderBuf))) {
				_shaderName = shaderBuf;
			}
			
			if (ImGui::Button("Reset to Default")) {
				_shaderName = "default";
			}
		}
	}

	void MeshRenderer::serialize(JsonSerializer& s) const {
		s.write("shader", _shaderName);
		s.write("meshType", _currentMeshType);
	}

	void MeshRenderer::deserialize(JsonSerializer& s) {
		_shaderName = s.readString("shader");
		_currentMeshType = s.readString("meshType");
	}
}
