#include <GLApplication.h>
#include <iostream>
#include <utils/logger.h>
#include <filesystem>

int main(int argc, char* argv[]) {
	std::filesystem::path rutaExe = std::filesystem::absolute(argv[0]);
	std::filesystem::path carpetaExe = rutaExe.parent_path();
	std::filesystem::current_path(carpetaExe);

	if (!cme::GLApplication::Init()) {
		LOG_ERROR("No se pudo inicializar GLApplication");
		return -1;
	}
	cme::gla().run();
	cme::gla().Release();

    return 0;
}