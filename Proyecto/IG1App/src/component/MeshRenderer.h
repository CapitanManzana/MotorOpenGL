#pragma once
#include <ec/component.h>
#include <string>
#include <glm/fwd.hpp>
#include <core/Material.h>

namespace cme {
	class Mesh;
	class Camera;
	class Transform;

	/// @brief Componente que se encarga de renderizar a partir de una malla dada
	class MeshRenderer : public ec::Component, public ec::RenderComponent {
	private:
		Mesh* _mesh = nullptr;
		Camera* _cam = nullptr;
		Transform* _tr = nullptr;
		std::string _currentMeshType;

	public:
		__CMPID_DECL__(ec::comp::MESH_RENDERER)

		MeshRenderer() : _mesh(nullptr) {}

		MeshRenderer(Mesh* mesh);
		~MeshRenderer();

		void render() const override;

		void initComponent() override;

		void getLocalAABB(glm::vec3& outMin, glm::vec3& outMax) const;

		void drawOnInspector() override;

		void serialize(JsonSerializer& s) const override;
		void deserialize(JsonSerializer& s) override;

		std::string serializeID() const override { return "MeshRenderer"; }

		void setLightSource(bool value);

		glm::mat3& normalMatrix();

		Material* material();
	};
}