#pragma once

namespace cme {
	/// @brief Interfaz para que las clases se puedan ver en el inspector
	class Inspectable
	{
	public:
		virtual ~Inspectable() = default;

		virtual void drawOnInspector() = 0;
	};
}
