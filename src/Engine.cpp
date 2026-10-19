//
// Created by Tyler Jaacks on 9/28/26.
//

#include "Engine.h"

#include <chrono>

namespace {
    float GetCurrentTimeSeconds() {
        using Clock = std::chrono::high_resolution_clock;

        const auto now = Clock::now();
        const auto duration = now.time_since_epoch();

        return std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
    }
}

Engine::Engine(std::string applicationName, const UserConfig& userConfig)
    : m_applicationName(std::move(applicationName)),
      m_userConfig(userConfig),
      m_window(new Window()),
      m_renderer(new Renderer()),
      m_layerStack(),
      m_activeScene(),
      m_Timestep(),
      m_running(false) {
}
Engine::~Engine() {
    Shutdown();

    delete m_renderer;
    m_renderer = nullptr;

    delete m_window;
    m_window = nullptr;
}
void Engine::Initialize() {
    WindowProperties properties;

    properties.Title = m_applicationName;
    properties.Width = static_cast<unsigned int>(m_userConfig.WindowWidth);
    properties.Height = static_cast<unsigned int>(m_userConfig.WindowHeight);
    properties.Fullscreen = m_userConfig.Fullscreen;
    properties.VSync = m_userConfig.VSync;

    m_window->Create(properties);

    m_window->SetEventCallback([this](Event& event) {
        OnEvent(event);
    });

    const RendererConfig rendererConfig{
        .backend = m_userConfig.Backend,
        .enableValidation = m_userConfig.EnableValidation
    };

    m_renderer->Initialize(*m_window, rendererConfig);

    m_running = true;
}

void Engine::Run() {
    Initialize();

    float lastFrameTime = GetCurrentTimeSeconds();

    while (m_running && !m_window->ShouldClose()) {
        const float currentFrameTime = GetCurrentTimeSeconds();

        m_Timestep = Timestep(currentFrameTime - lastFrameTime);

        lastFrameTime = currentFrameTime;

        Update(m_Timestep);
        Render();
    }

    Shutdown();
}

void Engine::Update(const Timestep timestep) {
    m_window->Update();

    for (Layer* layer : m_layerStack) {
        layer->OnUpdate(timestep);
    }

    m_activeScene.OnUpdate(timestep);
}

void Engine::Render() const {
    m_renderer->BeginFrame();
    m_renderer->EndFrame();
}

void Engine::Shutdown() const {
    if (m_renderer != nullptr) {
        m_renderer->Shutdown();
    }

    if (m_window != nullptr) {
        m_window->Shutdown();
    }
}

void Engine::OnEvent(Event &event) {

}
