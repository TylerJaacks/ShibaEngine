#ifndef SHIBAENGINE_MYGAME_H
#define SHIBAENGINE_MYGAME_H

#include <Game.h>
#include <ECS/Scene.h>

#include <memory>

class MyGame : public Game {
public:
    MyGame();

    void OnInitialize() override;
    void OnUpdate(Timestep timestep) override;
    void OnRender() override;
    void OnEvent(Event& event) override;
    void OnShutdown() override;

private:
    std::unique_ptr<Scene> m_scene;
};

#endif //SHIBAENGINE_MYGAME_H
