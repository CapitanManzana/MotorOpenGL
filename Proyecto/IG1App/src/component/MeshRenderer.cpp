#include "MeshRenderer.h"
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

MeshRenderer::~MeshRenderer() {
	delete _mesh; // Esto invocará Mesh::~Mesh() y liberará VBOs/VAOs y memoria
}

void MeshRenderer::render() const {
	if (!_mesh) return;
	_mesh->setModelMatrix(_tr->getModelMatrix());

	_cam->uploadToGPU(_mesh);
	_mesh->render();
}

void MeshRenderer::initComponent() {
	_cam = _entity->getScene()->getCamera();
	_tr = _entity->getComponent<Transform>();

	assert(_tr != nullptr);
}

void MeshRenderer::serialize(capiEngine::JsonSerializer& s) const {
	s.write("mesh", (int)_mesh->id());
}

void MeshRenderer::deserialize(capiEngine::JsonSerializer& s) {
	int meshID = s.readInt("mesh");
	if (meshID == 1) {
		_mesh = new TriangleMesh(rscrM().getShader("default"));
	}
	else if (meshID == 2) {
		_mesh = new QuadMesh(rscrM().getShader("default"));
	}
	else if (meshID == 3) {
		_mesh = new CubeMesh(rscrM().getShader("default"));
	}

	if (!_mesh) LOG_ERROR("La mesh es nula despues de cargarla del archivo");
}
