#include "windows/LightingWindow.h"
#include <lighting/GlobalLight.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <drawGUI/component/GlobalLightGUI.h>

namespace cme::editor {
	LightingWindow::LightingWindow(const char* name) : Window(name) {
		_lightGUI = std::make_unique<GlobalLightGUI>(cme::sceneM().activeScene()->globalLight());
	}

	void LightingWindow::renderWindowContent() {
		if (_lightGUI) _lightGUI->drawOnInspector();
	}
}