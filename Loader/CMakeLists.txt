add_executable(Loader
        src/main.cpp
)

target_link_libraries(Loader PRIVATE ShibaEngine)

# The game module is loaded at runtime, not linked, so build it alongside the Loader and pass its file name in.
add_dependencies(Loader Sandbox)

target_compile_definitions(Loader PRIVATE
        SHIBA_GAME_MODULE="$<TARGET_FILE_NAME:Sandbox>"
)

shiba_set_warnings(Loader)
