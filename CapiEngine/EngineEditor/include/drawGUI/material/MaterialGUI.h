#pragma once
#include <drawGUI/DrawGUI.h>

namespace cme {
	class Material;
}

namespace cme::editor {
	class MaterialGUI : public DrawGUI {
	private:
		Material* _material;

	public:
		MaterialGUI(Material* material);
		void drawOnInspector() override;
	};
}
