#ifndef SHIBAENGINE_MYSCENE_H
#define SHIBAENGINE_MYSCENE_H

#include <ECS/Scene.h>

#include "MyEntity.h"

class MyScene : public Scene {
public:
    ~MyScene() override;

    void OnInitialize() override;

    void OnUpdate(Timestep timestep) override;

    void OnRender() override;

    void OnShutdown() override;

private:
    Entity *m_Entity;
};

#endif //SHIBAENGINE_MYSCENE_H
