#include "MeshRenderer.h"
#include <ec/entity.h>

#include <core/Mesh.h>
#include <core/Camera.h>
#include <core/Scene.h>

#include <component/Transform.h>

MeshRenderer::~MeshRenderer() {
	delete _mesh; // Esto invocará Mesh::~Mesh() y liberará VBOs/VAOs y memoria
}

void MeshRenderer::render() const {
	_mesh->setModelMatrix(_tr->getModelMatrix());

	_cam->uploadToGPU(_mesh);
	_mesh->render();
}

void MeshRenderer::initComponent() {
	_cam = _entity->getScene()->getCamera();
	_tr = _entity->getComponent<Transform>();

	assert(_tr != nullptr);
}
