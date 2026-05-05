#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "EditorApp.h"
#include <iostream>
#include <utils/logger.h>
#include <filesystem>

int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::filesystem::path enginePath = std::filesystem::absolute(argv[0]).parent_path();
	std::filesystem::path projectPath;

	if (argc > 1) {
		std::filesystem::path argumentPath = argv[1];

		// Si hicieron doble clic en un archivo .capiproj
		if (argumentPath.extension() == ".capiproj") {
			// El projectPath es la carpeta DONDE EST   ese archivo
			projectPath = argumentPath.parent_path();
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

	
	if (!cme::editor::EditorApp::Init(enginePath, projectPath)) {
		LOG_ERROR("No se pudo inicializar el motor");
		return -1;
	}
	
	cme::editor::EditorApp::Instance()->run();
	cme::editor::EditorApp::Release();

	return 0;
}