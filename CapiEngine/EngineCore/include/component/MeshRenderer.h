#pragma once
#include <ec/component.h>
#include <string>
#include <array>
#include <glm/fwd.hpp>
#include <surface/Material.h>

namespace cme {
	class Mesh;
	class Camera;
	class Transform;

	/// @brief Componente que se encarga de renderizar a partir de una malla dada
	class MeshRenderer : public ec::Component, public ec::RenderComponent {
	public:
		std::unique_ptr<Mesh> _mesh = nullptr;
		Camera* _cam = nullptr;
		Transform* _tr = nullptr;
		std::string _currentMeshType;
		static constexpr std::array<const char*, 4> MESH_T_NAMES = { "None", "Triangle", "Quad", "Cube" };

		__CMPID_DECL__(ec::comp::MESH_RENDERER)

		MeshRenderer();
		MeshRenderer(Mesh* mesh);

		void render() const override;

		void initComponent() override;

		void getLocalAABB(glm::vec3& outMin, glm::vec3& outMax) const;

		void serialize(JsonSerializer& s) const override;
		void deserialize(JsonSerializer& s) override;

		std::string serializeID() const override { return "MeshRenderer"; }

		void setLightSource(bool value);

		glm::mat3& normalMatrix();

		Material* material();
	};
}