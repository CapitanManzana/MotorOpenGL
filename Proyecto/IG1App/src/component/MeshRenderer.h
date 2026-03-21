#pragma once
#include <ec/component.h>

class Mesh;
class Camera;
class Transform;

class MeshRenderer : public ec::Component, public ec::RenderComponent {
private:
	Mesh* _mesh;
	Camera* _cam;
	Transform* _tr;
public:
	__CMPID_DECL__(ec::comp::MESH_RENDERER)

	MeshRenderer(Mesh* mesh) : _mesh(mesh) {}
	~MeshRenderer();

	void render() const override;

	void initComponent() override;
};