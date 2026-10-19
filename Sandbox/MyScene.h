#ifndef SHIBAENGINE_MYSCENE_H
#define SHIBAENGINE_MYSCENE_H

#include <ECS/Scene.h>

class MyEntity;

class MyScene : public Scene {
public:
    ~MyScene() override;

    void OnInitialize() override;

private:
    // Owned by Scene; this is just a typed handle.
    MyEntity* m_entity = nullptr;
};

#endif //SHIBAENGINE_MYSCENE_H
