#ifndef SHIBAENGINE_SCENE_H
#define SHIBAENGINE_SCENE_H

#include <Core/Timestep.h>
#include <ECS/Entity.h>

#include <vector>

class Scene {
public:
    Scene();

    virtual ~Scene() = default;

    explicit Scene(const std::vector<Entity *> &entities);

    virtual void OnInitialize();
    virtual void OnUpdate(Timestep timestep);
    virtual void OnRender();
    virtual void OnShutdown();

    [[nodiscard]] const std::vector<Entity*>& GetEntities() const { return m_entities; }
    [[nodiscard]] std::vector<Entity*>& GetEntities() { return m_entities; }
private:
    std::vector<Entity*> m_entities;
};

#endif //SHIBAENGINE_SCENE_H
