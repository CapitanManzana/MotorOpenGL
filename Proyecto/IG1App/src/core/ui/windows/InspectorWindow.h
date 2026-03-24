#pragma once
#include <core/ui/Window.h>
#include <ec/entity.h>

namespace cme::ui {
	class InspectorWindow : public Window
	{
	private:
		ec::entity_t _selectedEnt = nullptr;
	public:
		WINDOW_ID(ui::windowGroupID::INSPECTOR)
		InspectorWindow(const char* name, ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse) :
			Window(name, flags) {}

		/// @brief Cambia el foco de la ventana a la entidad que se le pasa
		/// @param entity La entidad
		void changeDisplayEntity(ec::entity_t entity) { _selectedEnt = entity; }

	protected:
		virtual void renderWindowContent() override;
	};
}

