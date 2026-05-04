#include "windows/LightingWindow.h"
#include <lighting/GlobalLight.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <drawGUI/component/GlobalLightGUI.h>

namespace cme::editor {
	LightingWindow::LightingWindow(const char* name) : Window(name) {
	}

	void LightingWindow::renderWindowContent() {
		GlobalLightGUI l(cme::sceneM().activeScene()->globalLight());
		l.drawOnInspector();
	}
}