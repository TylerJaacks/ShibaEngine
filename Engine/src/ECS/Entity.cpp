#include <ECS/Entity.h>

Entity::Entity() : m_components() {
}

Entity::~Entity() = default;

void Entity::OnInitialize() {
    for (const auto& component : m_components) {
        component->OnInitialize();
    }
}

void Entity::OnUpdate(Timestep timestep) {
    for (const auto& component : m_components) {
        component->OnUpdate(timestep);
    }
}

void Entity::OnRender() {
    for (const auto& component : m_components) {
        component->OnRender();
    }
}

void Entity::OnShutdown() {
    for (const auto& component : m_components) {
        component->OnShutdown();
    }
}
