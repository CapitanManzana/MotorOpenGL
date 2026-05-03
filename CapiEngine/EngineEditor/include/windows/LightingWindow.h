#pragma once
#include <windows/Window.h>
#include <memory>

namespace cme::editor {
	class GlobalLightGUI;

	/// @brief Ventana que se encarga de renderizar el viewport
	class LightingWindow : public Window
	{
	private:
		std::unique_ptr<GlobalLightGUI> _lightGUI = nullptr;
	public:
		WINDOW_ID(windowGroupID::LIGHTING)
		LightingWindow(const char* name);

	protected:
		void renderWindowContent() override;
	};
}

