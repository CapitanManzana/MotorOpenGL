#pragma once
#include <core/ui/Window.h>

namespace capiEngine::ui {
	class InspectorWindow : public Window
	{
	public:
		WINDOW_ID(capiEngine::ui::groupID::INSPECTOR)
		InspectorWindow(const char* name, WindowFrame frame = WindowFrame(), ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove) :
			Window(name, frame, flags) {}

	protected:
		virtual void renderWindowContent() const override;
	};
}

