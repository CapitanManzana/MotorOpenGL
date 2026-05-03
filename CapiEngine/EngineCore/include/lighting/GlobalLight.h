#pragma once
#include <core/serialize/Inspectable.h>
#include <core/serialize/Serializable.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace cme {
	class GlobalLight : public Inspectable, public Serializable
	{
	private:
		glm::vec3 _color = glm::vec3(1.f);
		glm::vec3 _dir = glm::vec3(0.577, -0.577, 0.577);
		float _intensity = 1.f;

		float _azimuth = 45.0f;  // grados, horizontal
		float _elevation = -45.0f; // grados, vertical (negativo = desde arriba)
	public:
		GlobalLight() = default;

		void drawOnInspector() override;

		const glm::vec3& color() const { return _color; }
		const glm::vec3& direction() const { return _dir; }
		float intensity() const { return _intensity; }

		void serialize(JsonSerializer& s) const override;
		void deserialize(JsonSerializer& s) override;
	};
}

