# COMPILAR LUA COMO LIBRERÍA ESTÁTICA
set(LUA_DIR "${CMAKE_SOURCE_DIR}/vendor/lua/src")

# Recoger todos los archivos .c de Lua
file(GLOB LUA_SOURCES "${LUA_DIR}/*.c")

# Excluir los ejecutables standalone de Lua 
# porque tienen funciones main() y romperían la compilación.
list(REMOVE_ITEM LUA_SOURCES 
    "${LUA_DIR}/lua.c" 
    "${LUA_DIR}/luac.c"
)

# Crear la librería estática de Lua
add_library(lua_static STATIC ${LUA_SOURCES})

# Decirle a la librería dónde están sus propios headers
target_include_directories(lua_static PUBLIC ${LUA_DIR})
set_target_properties(lua_static PROPERTIES LINKER_LANGUAGE C)

# CONFIGURACIÓN ENGINECORE
# Buscar todos los .cpp dentro de src y sus subcarpetas
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")

# Buscar todos los .h y .hpp dentro de include y sus subcarpetas
file(GLOB_RECURSE CORE_HEADERS "include/*.h" "include/*.hpp")

# Crear la librería incluyendo los sources Y los headers
add_library(EngineCore STATIC 
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ../vendor/glad/src/glad.c
)

# Exponer la carpeta include públicamente
target_include_directories(EngineCore PUBLIC 
    include
    ../vendor/glad/include
    ../vendor/glm/include
    ../vendor/stb_image/include
    ../vendor/glfw/include
    ../vendor/sol/include
    ../vendor/lua/src
)

target_compile_definitions(EngineCore PUBLIC 
    GLFW_INCLUDE_NONE
    SOL_ALL_SAFETIES_ON=1
)

target_link_libraries(EngineCore PUBLIC 
    "${CMAKE_SOURCE_DIR}/vendor/glfw/lib/glfw3dll.lib" 
    lua_static                  # Enlazamos la librería que acabamos de compilar arriba
)

# Copiar la DLL al lado del ejecutable automáticamente
add_custom_command(TARGET EngineCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vendor/glfw/bin/glfw3.dll"
        "$<TARGET_FILE_DIR:EngineEditor>"
)