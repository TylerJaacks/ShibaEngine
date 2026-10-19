#ifndef SHIBAENGINE_MYENTITY_H
#define SHIBAENGINE_MYENTITY_H

#include <ECS/Entity.h>

class MyEntity : public Entity {
public:
    ~MyEntity() override;

    void OnInitialize() override;

    void OnUpdate(Timestep timestep) override;

    void OnRender() override;

    void OnShutdown() override;

private:
    Component* m_cameraComponent = nullptr;
    Component* m_meshComponent = nullptr;
    Component* m_transformComponent = nullptr;
};

#endif //SHIBAENGINE_MYENTITY_H
