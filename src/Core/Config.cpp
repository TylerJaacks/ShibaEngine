#include "Core/Config.h"

#include <fstream>
#include <sstream>
#include <string>

namespace {
    bool Contains(const std::string& source, const std::string& value) {
        return source.find(value) != std::string::npos;
    }

    RHIBackend GetDefaultBackend() {
#if defined(__APPLE__)
        return RHIBackend::Metal;
#elif defined(_WIN32)
        return RHIBackend::DirectX12;
#else
        return RHIBackend::Vulkan;
#endif
    }

    RHIBackend ParseBackend(const std::string& contents) {
        if (Contains(contents, "\"Metal\"")) {
            return RHIBackend::Metal;
        }

        if (Contains(contents, "\"DirectX12\"")) {
            return RHIBackend::DirectX12;
        }

        if (Contains(contents, "\"Vulkan\"")) {
            return RHIBackend::Vulkan;
        }

        if (Contains(contents, "\"OpenGL\"")) {
            return RHIBackend::OpenGL;
        }

        if (Contains(contents, "\"Default\"")) {
            return GetDefaultBackend();
        }

        return GetDefaultBackend();
    }
}

UserConfig Config::LoadUserConfig(const char* path) {
    UserConfig config{
        .WindowWidth = DefaultWindowWidth,
        .WindowHeight = DefaultWindowHeight,
        .Fullscreen = DefaultFullscreen,
        .VSync = DefaultVSync,
        .EnableValidation = DefaultEnableValidation,
        .Backend = GetDefaultBackend()
    };

    std::ifstream file(path);

    if (!file.is_open()) {
        return config;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    const std::string contents = buffer.str();

    config.Backend = ParseBackend(contents);

    return config;
}