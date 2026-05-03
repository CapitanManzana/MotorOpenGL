#pragma once
#include <GLApplication.h>
#include "managers/UIManager.h"
#include <managers/SceneManager.h>
#include <managers/InputManager.h>

#include <core/Scene.h>
#include <utils/logger.h>

namespace cme::editor {
	class EditorApp {
	private:
		std::shared_ptr<UIManager> _ui;

	public:
		EditorApp() = default;
		~EditorApp() {
			if (GLApplication::HasInstance()) {
				GLApplication::Release();
			}
		}

		bool init();
		void run();

		void createShortcuts();
		void stateChangers();
	};
}
