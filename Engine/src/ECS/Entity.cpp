#include <ECS/Entity.h>

Entity::Entity() : m_components() {
}

Entity::~Entity() = default;

void Entity::OnInitialize() {
    for (Component *component : GetComponents()) {
        component->OnInitialize();
    }
}

void Entity::OnUpdate(Timestep timestep) {
    for (Component *component : GetComponents()) {
        component->OnUpdate(timestep);
    }
}

void Entity::OnRender() {
    for (Component *component : GetComponents()) {
        component->OnRender();
    }
}

void Entity::OnShutdown() {
    for (Component *component : GetComponents()) {
        component->OnShutdown();
    }
}