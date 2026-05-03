# 1. Definir la ruta de ImGui relativa a este archivo
# Si EngineEditor y vendor están al mismo nivel, la ruta es ../vendor/imgui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui")

# 2. Archivos fuente de ImGui (Escríbelos a mano para evitar errores de GLOB)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
)

# 3. Recoger tus archivos del Editor
file(GLOB_RECURSE EDITOR_SOURCES 
    "src/*.cpp" 
    "src/*.h"
    "include/*.h"
)

# 4. Crear el ejecutable 
# IMPORTANTE: Verifica que ${EDITOR_SOURCES} no esté vacío (que tengas al menos un .cpp en src)
add_executable(EngineEditor 
    ${EDITOR_SOURCES} 
    ${IMGUI_SOURCES}
)

set_target_properties(EngineEditor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# 5. Directorios de inclusión
target_include_directories(EngineEditor PRIVATE 
    src
    include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_compile_definitions(EngineEditor PRIVATE GLFW_INCLUDE_NONE)

# 6. Enlazar con el Core
target_link_libraries(EngineEditor PRIVATE EngineCore)