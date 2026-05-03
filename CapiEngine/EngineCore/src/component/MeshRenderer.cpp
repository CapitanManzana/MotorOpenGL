#include <component/MeshRenderer.h>

#include <ec/entity.h>

#include <mesh/Mesh.h>
#include <core/Camera.h>
#include <core/Scene.h>

#include <component/Transform.h>
#include <serialize/JsonSerializer.h>
#include <mesh/CubeMesh.h>
#include <mesh/TriangleMesh.h>
#include <mesh/QuadMesh.h>
#include <managers/ResourceManager.h>

namespace cme {
	MeshRenderer::MeshRenderer(Mesh* mesh) : _mesh(mesh) {
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
		s.beginScope("material");
		_mesh->material()->serialize(s);
		s.endScope();
	}

	void MeshRenderer::deserialize(JsonSerializer& s) {
		int meshID = s.readInt("mesh");

		if (meshID == 1)      _mesh = new TriangleMesh();
		else if (meshID == 2) _mesh = new QuadMesh();
		else if (meshID == 3) _mesh = new CubeMesh();

		if (!_mesh) {
			LOG_ERROR("La mesh es nula despues de cargarla del archivo");
			return;
		}

		if (auto entitySp = _entity.lock()) {
			if (meshID >= 0 && meshID < MESH_T_NAMES.size())
				_currentMeshType = MESH_T_NAMES[meshID];
		}

		s.beginScope("material");
		_mesh->material()->deserialize(s);
		s.endScope();
	}

}