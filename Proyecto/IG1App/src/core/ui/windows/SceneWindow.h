#pragma once
#include <core/ui/Window.h>
#include <ec/entity.h>

namespace capiEngine::ui {
	class SceneWindow : public Window
	{
	private:
		std::function<void(ec::entity_t)> _selectedEntCallback = nullptr;
	public:
		WINDOW_ID(capiEngine::ui::windowGroupID::SCENE)

		SceneWindow(const char* name, ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse) :
			Window(name, flags) {
		}

		/// @brief Establece la callback al seleccionar una entidad. Es de tipo void(ec::entity_t) porque se le pasa la entidad
		/// @param call La callback a realizar
		void setCallback(std::function<void(ec::entity_t)> call) { _selectedEntCallback = call; }
	protected:
		virtual void renderWindowContent() override;
	};
}