#pragma once
#include <drawGUI/DrawGUI.h>

namespace cme {
	class Light;
}

namespace cme::editor {
	class LightGUI : public DrawGUI {
	private:
		Light* _light;

	public:
		LightGUI(Light* light);
		void drawOnInspector() override;
	};
}
