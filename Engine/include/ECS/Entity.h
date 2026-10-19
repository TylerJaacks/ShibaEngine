#ifndef SHIBAENGINE_ENTITY_H
#define SHIBAENGINE_ENTITY_H

#include <vector>

#include <Core/Timestep.h>
#include <ECS/Component.h>

class Entity {
public:
    Entity();
    ~Entity();

    void OnInitialize();
    void OnUpdate(Timestep timestep);
    void OnRender();
    void OnShutdown();

    [[nodiscard]] const std::vector<Component*>& GetComponents() const;
private:
    std::vector<Component*> m_components;
};

#endif //SHIBAENGINE_ENTITY_H