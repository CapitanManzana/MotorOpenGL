#pragma once
#include <core/ui/Window.h>
#include <ec/entity.h>

namespace cme::ui {
	/// @brief Ventana que se encarga de mostrar la información de una entidad seleccionada
	class InspectorWindow : public Window
	{
	private:
		std::weak_ptr<ec::Entity> _selectedEnt;
	public:
		WINDOW_ID(ui::windowGroupID::INSPECTOR)
		InspectorWindow(const char* name, ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse) :
			Window(name, flags) {}

		/// @brief Cambia el foco de la ventana a la entidad que se le pasa
		/// @param entity La entidad
		void changeDisplayEntity(std::weak_ptr<ec::Entity> entity) { _selectedEnt = entity; }

	protected:
		virtual void renderWindowContent() override;
	};
}

