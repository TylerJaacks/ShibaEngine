file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(Sandbox SHARED
        ${SANDBOX_SOURCES}
)

target_include_directories(Sandbox PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(Sandbox PRIVATE ShibaEngine)

shiba_set_warnings(Sandbox)
