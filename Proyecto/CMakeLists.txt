#
# Proyecto CMake para las prácticas de GC
#

cmake_minimum_required(VERSION 3.15...3.30)

# Proyecto en C++
project(ProyectoIG VERSION 1.0 LANGUAGES CXX)

# Selecciona la versión C++20 del estándar (sin extensiones)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Genera el ejecutable en el subdirectorio bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# Compila con las bibliotecas GLEW, GLFW y GLM
if (CMAKE_HOST_WIN32)
	# Utiliza las bibliotecas compiladas que van incluidas
	# en el paquete con el proyecto de Visual Studio

	include_directories(
		libs/glew/include
		libs/glfw/include
		libs/glm/include
		libs/assimp/include
	)

	link_directories(
		libs/glew/lib
		libs/glfw/lib
		libs/assimp/lib/x64
	)

	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		link_libraries(assimp-vc143-mtd.lib)
	else()
		link_libraries(assimp-vc143-mt.lib)
	endif()

	link_libraries(
		glew32.lib
		glfw3dll.lib
		Opengl32.lib
	)

	# Añade checkML.h automáticamente a todas las unidades de compilación
	add_compile_options(/FI ${CMAKE_SOURCE_DIR}/libs/misc/include/CheckML.h)
else()
	# En otros sistemas operativos utiliza find_package
	# para encontrar las bibliotecas instaladas aparte

	find_package(glfw3 REQUIRED)
	find_package(glm REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(OpenGL REQUIRED)

	link_libraries(glfw GLEW::GLEW OpenGL::GL)
endif()

# Compila con stb_image
include_directories(libs/stb_image/include)

add_executable(IG1App
	# Archivo de código
	IG1App/Camera.cpp
	IG1App/Entity.cpp
	IG1App/IG1App.cpp
	IG1App/Image.cpp
	IG1App/main.cpp
	IG1App/Mesh.cpp
	IG1App/Scene.cpp
	IG1App/Shader.cpp
	IG1App/Texture.cpp
	IG1App/Viewport.cpp
)
