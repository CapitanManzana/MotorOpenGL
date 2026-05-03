#pragma once
#include <core/ui/Window.h>
#include <ec/entity.h>

namespace cme::ui {
	/// @brief Ventana que se encarga de mostrar la jerarquia de entidades de la escena y permite seleccionar una entidad para mostrarla en el inspector
	class SceneWindow : public Window
	{
	private:
		std::function<void(std::weak_ptr<ec::Entity>)> _selectedEntCallback = nullptr;
	public:
		WINDOW_ID(ui::windowGroupID::SCENE)

		SceneWindow(const char* name, ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse) :
			Window(name, flags) {
		}

		/// @brief Establece la callback al seleccionar una entidad. Es de tipo void(ec::entity_t) porque se le pasa la entidad
		/// @param call La callback a realizar
		void setCallback(std::function<void(std::weak_ptr<ec::Entity>)> call) { _selectedEntCallback = call; }
	protected:
		virtual void renderWindowContent() override;
	};
}