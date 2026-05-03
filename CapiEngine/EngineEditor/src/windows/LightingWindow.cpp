#include "LightingWindow.h"
#include <core/lighting/GlobalLight.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>

namespace cme::ui {
	LightingWindow::LightingWindow(const char* name) : Window(name) {
	}

	void LightingWindow::renderWindowContent() {
		sceneM().activeScene()->globalLight()->drawOnInspector();
	}
}