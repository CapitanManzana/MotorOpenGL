#pragma once
#include <drawGUI/DrawGUI.h>

namespace cme {
	class GlobalLight;
}

namespace cme::editor {
	class GlobalLightGUI : public DrawGUI {
	private:
		GlobalLight* _globalLight;

	public:
		GlobalLightGUI(GlobalLight* globalLight);
		void drawOnInspector() override;
	};
}
