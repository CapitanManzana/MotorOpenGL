#pragma once
#include <drawGUI/DrawGUI.h>

namespace cme {
	class MeshRenderer;
}

namespace cme::editor {
	class MeshRendererGUI : public DrawGUI {
	private:
		MeshRenderer* _meshRenderer;

	public:
		MeshRendererGUI(MeshRenderer* meshRenderer);
		void drawOnInspector() override;
	};
}
