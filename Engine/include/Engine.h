#ifndef SHIBAENGINE_ENGINE_H
#define SHIBAENGINE_ENGINE_H

#include <Core/Config.h>
#include <Core/LayerStack.h>
#include <Core/Timestep.h>
#include <ECS/Scene.h>
#include <Events/Event.h>
#include <Game.h>
#include <Platform/Window.h>
#include <Renderer/Renderer.h>

class Engine {
public:
    Engine(Game& game, const UserConfig& userConfig);
    ~Engine();

    void Initialize();
    void Run();
    void Update(Timestep timestep);

    void Render() const;

    void Shutdown() const;

    void OnEvent(Event& event);

    [[nodiscard]] bool IsRunning() const { return m_running; }

    [[nodiscard]] Window& GetWindow() const { return *m_window; }

private:
    Game& m_game;
    UserConfig m_userConfig;

    Window* m_window;
    Renderer* m_renderer;
    LayerStack m_layerStack;

    Scene m_activeScene;
    Timestep m_Timestep;

    bool m_running;
};

#endif //SHIBAENGINE_ENGINE_H