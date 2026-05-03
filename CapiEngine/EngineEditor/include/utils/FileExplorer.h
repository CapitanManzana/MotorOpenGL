#pragma once
#include <windows.h>
#include <commdlg.h>
#include <string>

namespace cme {
    enum class FileDialogMode { Open, Save };
	/// @brief Sirve para abrir el explorador de archivos para guardar/leer el path de algun archivo
	class FileExplorer {
	public:
		FileExplorer() {}

        /// @brief Abre la ventana para seleccionar una ruta o archivo
        /// @return La ruta seleccionada
        std::string fileDialog(FileDialogMode mode, const char* filter = "Scene Files\0*.json\0All Files\0*.*\0") {
            OPENFILENAMEA ofn;
            char filePath[MAX_PATH] = "";

            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = filePath;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrFilter = filter;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (mode == FileDialogMode::Open) {
                if (GetOpenFileNameA(&ofn)) return filePath;
            }
            else {
                ofn.lpstrDefExt = "json"; // extensión por defecto al guardar
                if (GetSaveFileNameA(&ofn)) return filePath;
            }

            return "";
        }
	};
}