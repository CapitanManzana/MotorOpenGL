#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <core/GLApplication.h>
#include <iostream>
#include <utils/logger.h>
#include <filesystem>

int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(900);

	std::filesystem::path rutaExe = std::filesystem::absolute(argv[0]);
	std::filesystem::path carpetaExe = rutaExe.parent_path();
	std::filesystem::current_path(carpetaExe);

	if (!cme::GLApplication::Init()) {
		LOG_ERROR("No se pudo inicializar GLApplication");
		return -1;
	}
	cme::gla().start();
	cme::gla().Release();

	return 0;
}

