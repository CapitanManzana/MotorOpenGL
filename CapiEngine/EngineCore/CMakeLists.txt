# 1. Buscar todos los .cpp dentro de src y sus subcarpetas
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")

# 2. Buscar todos los .h y .hpp dentro de include y sus subcarpetas
# (Añadimos .hpp por si acaso, ya que json.hpp lo usa)
file(GLOB_RECURSE CORE_HEADERS "include/*.h" "include/*.hpp")

# 3. Crear la librería incluyendo los sources Y los headers
add_library(EngineCore STATIC 
    ${CORE_SOURCES} 
    ${CORE_HEADERS}
    ../vendor/glad/src/glad.c  # Mantenemos glad
)

# 4. Exponer la carpeta include públicamente
target_include_directories(EngineCore PUBLIC 
    include
    ../vendor/glad/include
    ../vendor/glad/include
    ../vendor/glm/include
    ../vendor/stb_image/include
    ../vendor/glfw/include
)

target_compile_definitions(EngineCore PUBLIC GLFW_INCLUDE_NONE)

target_link_libraries(EngineCore PUBLIC 
    "${CMAKE_SOURCE_DIR}/vendor/glfw/lib/glfw3dll.lib"  # ← nombre correcto
)

# Copiar la DLL al lado del ejecutable automáticamente
add_custom_command(TARGET EngineCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vendor/glfw/bin/glfw3.dll"
        "$<TARGET_FILE_DIR:EngineEditor>"
)