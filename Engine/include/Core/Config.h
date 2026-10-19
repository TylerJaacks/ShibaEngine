#ifndef SHIBAENGINE_CONFIG_H
#define SHIBAENGINE_CONFIG_H

#include <Renderer/RHI/RHI.h>

struct UserConfig {
    int WindowWidth = 1280;
    int WindowHeight = 720;
    bool Fullscreen = false;
    bool VSync = true;
    bool EnableValidation = true;
    RHIBackend Backend = RHIBackend::None;
};

namespace Config {
    inline constexpr int DefaultWindowWidth = 1280;
    inline constexpr int DefaultWindowHeight = 720;
    inline constexpr bool DefaultFullscreen = false;
    inline constexpr bool DefaultVSync = true;
    inline constexpr bool DefaultEnableValidation = true;

    UserConfig LoadUserConfig(const char* path);
}

#endif //SHIBAENGINE_CONFIG_H