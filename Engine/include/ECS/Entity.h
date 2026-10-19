#ifndef SHIBAENGINE_ENTITY_H
#define SHIBAENGINE_ENTITY_H

#include <vector>

#include <Core/Timestep.h>
#include <ECS/Component.h>

class Entity {
public:
    Entity();

    virtual ~Entity();

    virtual void OnInitialize();
    virtual void OnUpdate(Timestep timestep);
    virtual void OnRender();
    virtual void OnShutdown();

    [[nodiscard]] const std::vector<Component*>& GetComponents() const { return m_components; }
    [[nodiscard]] std::vector<Component*>& GetComponents() { return m_components; }
private:
    std::vector<Component*> m_components;
};

#endif //SHIBAENGINE_ENTITY_H