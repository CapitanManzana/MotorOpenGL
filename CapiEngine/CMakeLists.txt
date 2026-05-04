cmake_minimum_required(VERSION 3.20)
project(CapiEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Añadimos los subproyectos
add_subdirectory(EngineCore)
add_subdirectory(EngineEditor)
add_subdirectory(Runtime)