#ifndef SHIBAENGINE_SCENE_H
#define SHIBAENGINE_SCENE_H

#include <Core/Timestep.h>
#include <ECS/Entity.h>

#include <vector>

class Scene {
public:
    Scene();

    explicit Scene(const std::vector<Entity *> &entities);

    void OnInitialize() const;
    void OnUpdate(Timestep timestep) const;
    void OnRender() const;
    void OnShutdown() const;

    [[nodiscard]] std::vector<Entity*> GetEntities();
private:
    std::vector<Entity*> m_entities;
};

#endif //SHIBAENGINE_SCENE_H
