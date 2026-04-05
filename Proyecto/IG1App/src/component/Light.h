#pragma once
#include <ec/component.h>
#include <glm/vec4.hpp> 
#include <component/Transform.h>

namespace cme {

	/// @brief Componente que sirve para poner una luz en la escena
	class Light : public ec::Component
	{
	private:
		glm::vec3 _color = { 1.f, 1.f, 1.f };
		float _intensity = 1.f;
		float _ambienteStrength = 0.1f;

		Transform* _tr;

		static std::vector<Light*> _allLights;
	public:
		__CMPID_DECL__(ec::comp::LIGHT)

		~Light();
		void initComponent() override;
		static const std::vector<Light*>& getAllLights() { return _allLights; }


		void drawOnInspector() override;

		void serialize(JsonSerializer& s) const override;
		void deserialize(JsonSerializer& s) override;

		std::string serializeID() const override { return "Light"; }

		const glm::vec3& color() const { return _color; }
		float intensity() const { return _intensity; }
		float ambientStrength() const { return _ambienteStrength; }

		void setColor(const glm::vec4& color) { _color = color; }
		void setIntensity(float intensity) { _intensity = intensity; }
		void setAmbientStrength(float ams) { _ambienteStrength = ams; }

		glm::vec3 getPosition() const { return _tr->getPosition(); }
	};
}
