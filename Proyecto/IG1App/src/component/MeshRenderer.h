#pragma once
#include <ec/component.h>
#include <core/serialize/Serializable.h>

class Mesh;
class Camera;
class Transform;

class MeshRenderer : public ec::Component, public ec::RenderComponent, public capiEngine::Serializable {
private:
	Mesh* _mesh = nullptr;
	Camera* _cam = nullptr;
	Transform* _tr = nullptr;
public:
	__CMPID_DECL__(ec::comp::MESH_RENDERER)

	MeshRenderer() : _mesh(nullptr) {}
	MeshRenderer(Mesh* mesh) : _mesh(mesh) {}
	~MeshRenderer();

	void render() const override;

	void initComponent() override;

	virtual void serialize(capiEngine::JsonSerializer& s) const override;
	virtual void deserialize(capiEngine::JsonSerializer& s) override;
};