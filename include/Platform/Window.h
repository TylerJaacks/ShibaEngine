//
// Created by Tyler Jaacks on 9/28/26.
//

#ifndef SHIBAENGINE_WINDOW_H
#define SHIBAENGINE_WINDOW_H

#include "Events/Event.h"
#include "Platform/NativeWindowHandle.h"

#include <functional>
#include <string>

struct WindowProperties {
    std::string Title = "ShibaEngine";
    unsigned int Width = 1280;
    unsigned int Height = 720;
    bool Fullscreen = false;
    bool VSync = true;
};

class Window {
public:
    using EventCallbackFunction = std::function<void(Event&)>;

    Window();
    ~Window();

    void Create(const WindowProperties& properties);
    void Update();
    void Shutdown();

    void SetEventCallback(const EventCallbackFunction& callback);
    void SetVSync(bool enabled);

    void RequestClose();

    [[nodiscard]] unsigned int GetWidth() const;
    [[nodiscard]] unsigned int GetHeight() const;

    [[nodiscard]] bool IsVSync() const;
    [[nodiscard]] bool ShouldClose() const;

    [[nodiscard]] NativeWindowHandle GetNativeHandle() const;

private:
    struct PlatformData;

    WindowProperties m_Properties;
    EventCallbackFunction m_EventCallback;
    PlatformData* m_PlatformData = nullptr;

    bool m_shouldClose = false;
};

#endif //SHIBAENGINE_WINDOW_H