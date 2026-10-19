file(GLOB_RECURSE SHIBAENGINE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm"
)

file(GLOB_RECURSE SHIBAENGINE_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(ShibaEngine SHARED
        ${SHIBAENGINE_SOURCES}
        ${SHIBAENGINE_HEADERS}
)

# Export the whole engine API from the .dll without annotating every class.
set_target_properties(ShibaEngine PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_include_directories(ShibaEngine
        PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(DEFINED VCPKG_INSTALLED_DIR AND DEFINED VCPKG_TARGET_TRIPLET)
    target_include_directories(ShibaEngine PUBLIC
            "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include"
    )
endif()

target_link_libraries(ShibaEngine PRIVATE ${CMAKE_DL_LIBS})

if(APPLE)
    target_link_libraries(ShibaEngine PRIVATE
            "-framework Foundation"
            "-framework Metal"
            "-framework QuartzCore"
            "-framework AppKit"
    )
endif()

shiba_set_warnings(ShibaEngine)
