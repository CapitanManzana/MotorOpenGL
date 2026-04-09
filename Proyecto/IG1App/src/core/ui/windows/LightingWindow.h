#pragma once
#include <core/ui/Window.h>

namespace cme {
	class GlobalLight;
}

namespace cme::ui {
	/// @brief Ventana que se encarga de renderizar el viewport
	class LightingWindow : public Window
	{
	private:
		GlobalLight* _gL;
	public:
		WINDOW_ID(ui::windowGroupID::LIGHTING)
			LightingWindow(const char* name);

	protected:
		void renderWindowContent() override;
	};
}

