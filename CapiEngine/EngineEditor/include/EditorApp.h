#pragma once
#include <GLApplication.h>
#include "managers/UIManager.h"

namespace cme::editor {
    class EditorApp : public cme::GLApplication {
    private:
        std::unique_ptr<UIManager> _ui;

    public:
        EditorApp() = default;
        ~EditorApp() {
            delete m_Interface;
        }

        // El Core nos llamará aquí justo antes de empezar el bucle while
        void OnInitUI() {
            m_Interface = new cme::ui::UIManager();
            m_Interface->initCoreUI(window());

            // Configuramos eventos de la UI
            m_Interface->setCreateCubeCallback([]() {
                cme::sceneM().activeScene()->addCubeToScene();
                });
        }

        // El Core nos llamará aquí en cada frame, después de renderizar la escena 3D
        void OnRenderUI() {
            // Le decimos a ImGui que empiece un nuevo frame
            m_Interface->bind();

            // --- Aquí dibujarías tus paneles (Inspector, Jerarquía, etc.) ---
            // Ejemplo: Editor::DrawEntityProperties(entidadSeleccionada);

            // Renderizamos la UI en la pantalla
            m_Interface->render();
            m_Interface->unbind();
        }
    };
}
