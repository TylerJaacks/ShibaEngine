cmake_minimum_required(VERSION 4.3)

project(ShibaEngine
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SHIBAENGINE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm"
)

file(GLOB_RECURSE SHIBAENGINE_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_executable(ShibaEngine
        ${SHIBAENGINE_SOURCES}
        ${SHIBAENGINE_HEADERS}
        src/Renderer/Backend/Metal/MetalContext.cpp
)

target_include_directories(ShibaEngine PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(DEFINED VCPKG_INSTALLED_DIR AND DEFINED VCPKG_TARGET_TRIPLET)
    target_include_directories(ShibaEngine PRIVATE
            "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include"
    )
endif()

if(APPLE)
    target_link_libraries(ShibaEngine PRIVATE
            "-framework Foundation"
            "-framework Metal"
            "-framework QuartzCore"
            "-framework AppKit"
    )
endif()

if(MSVC)
    target_compile_options(ShibaEngine PRIVATE
            /W4
    )
else()
    target_compile_options(ShibaEngine PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()