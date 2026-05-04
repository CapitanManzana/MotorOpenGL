#pragma once
#include <windows/Window.h>
#include <memory>

namespace cme::editor {
	/// @brief Ventana que se encarga de renderizar el viewport
	class LightingWindow : public Window
	{
	public:
		WINDOW_ID(windowGroupID::LIGHTING)
		LightingWindow(const char* name);

	protected:
		void renderWindowContent() override;
	};
}

