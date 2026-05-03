#pragma once
#include <windows/Window.h>
#include <ec/entity.h>
#include <typeindex>
#include <unordered_map>
#include <functional>

namespace cme::editor {
	using ComponentDrawMap = std::unordered_map<std::type_index, std::function<void(ec::Component*)>>;

	/// @brief Ventana que se encarga de mostrar la información de una entidad seleccionada
	class InspectorWindow : public Window
	{
	private:
		ComponentDrawMap _componentUIRegistry;

		std::weak_ptr<ec::Entity> _selectedEnt;
	public:
		WINDOW_ID(windowGroupID::INSPECTOR)
		InspectorWindow(const char* name, ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse) :
			Window(name, flags) {
			registerUIComponents();
		}

		/// @brief Cambia el foco de la ventana a la entidad que se le pasa
		/// @param entity La entidad
		void changeDisplayEntity(std::weak_ptr<ec::Entity> entity) { _selectedEnt = entity; }

	protected:
		virtual void renderWindowContent() override;

		void registerUIComponents();
	};
}

