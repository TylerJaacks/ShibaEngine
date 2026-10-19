vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO apple/metal-cpp
        REF main
        SHA512 92eebd991a4614ef86f988d52a4303d2bfb042d19567e93bc946cfd7992a5a742b4dea7a0b037ccbc470e98816fbb51439b142672ab55d724135c7e84440d50b
        HEAD_REF main
)

file(INSTALL
        "${SOURCE_PATH}/Metal"
        "${SOURCE_PATH}/Foundation"
        "${SOURCE_PATH}/QuartzCore"
        DESTINATION "${CURRENT_PACKAGES_DIR}/include"
)

file(INSTALL
        "${SOURCE_PATH}/LICENSE.txt"
        DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
        RENAME copyright
)