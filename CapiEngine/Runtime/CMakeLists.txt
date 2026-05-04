# EngineRuntime/CMakeLists.txt
file(GLOB_RECURSE RUNTIME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(EngineRuntime ${RUNTIME_SOURCES})

target_link_libraries(EngineRuntime PRIVATE EngineCore)

# Copiar assets al lado del exe al compilar
add_custom_command(TARGET EngineRuntime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:EngineRuntime>/assets"
)

add_custom_command(TARGET EngineRuntime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vendor/glfw/bin/glfw3.dll"
        "$<TARGET_FILE_DIR:EngineRuntime>"
)