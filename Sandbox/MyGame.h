#ifndef SHIBAENGINE_MYGAME_H
#define SHIBAENGINE_MYGAME_H

#include <Game.h>
#include <ECS/Scene.h>

class MyGame : public Game {
public:
    MyGame();

    void OnInitialize() override;
    void OnUpdate(Timestep timestep) override;
    void OnRender() override;
    void OnEvent(Event& event) override;
    void OnShutdown() override;

private:
    Scene* m_Scene;
};

#endif //SHIBAENGINE_MYGAME_H
