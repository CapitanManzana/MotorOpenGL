#pragma once
#include <drawGUI/DrawGUI.h>

namespace cme {
	class Transform;
}

namespace cme::editor {
	class TransformGUI : public DrawGUI {
	private:
		Transform* _transform;

	public:
		TransformGUI(Transform* transform);
		void drawOnInspector() override;
	};
}
