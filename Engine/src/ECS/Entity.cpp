#include <ECS/Entity.h>

Entity::Entity() : m_components() {
}

Entity::~Entity() {

}

void Entity::OnInitialize() {
}

void Entity::OnUpdate(Timestep timestep) {
}

void Entity::OnRender() {
}

void Entity::OnShutdown() {
}

const std::vector<Component *> & Entity::GetComponents() const {
    return m_components;
}
