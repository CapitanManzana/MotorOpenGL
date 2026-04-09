#pragma once
#include <core/serialize/Inspectable.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace cme {
	class GlobalLight : public Inspectable
	{
	private:
		glm::vec3 _color = glm::vec3(1.f);
		glm::vec3 _dir = glm::vec3(0.577, -0.577, 0.577);
		float _intensity = 1.f;

	public:
		GlobalLight() = default;

		void drawOnInspector() override;

		const glm::vec3& color() const { return _color; }
		const glm::vec3& direction() const { return _dir; }
		float intensity() const { return _intensity; }
	};
}

