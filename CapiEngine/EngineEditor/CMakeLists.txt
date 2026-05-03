# 1. Buscar todos los .cpp dentro de la carpeta src del Editor
file(GLOB_RECURSE EDITOR_SOURCES "src/*.cpp")

# 2. Buscar todos los .h dentro del Editor (para que aparezcan en Visual Studio)
# Nota: Si los .h del editor los pones junto a los .cpp en 'src', cambia "include/*.h" por "src/*.h"
file(GLOB_RECURSE EDITOR_HEADERS "include/*.h" "include/*.hpp")

# 3. Creamos el ejecutable pasándole las variables (¡aquí está la corrección!)
add_executable(EngineEditor 
    ${EDITOR_SOURCES} 
    ${EDITOR_HEADERS}
)

# 4. ¡La magia! Vinculamos el Editor con el Core
target_link_libraries(EngineEditor PRIVATE EngineCore)