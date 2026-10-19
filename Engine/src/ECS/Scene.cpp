#include <ECS/Scene.h>

Scene::Scene() : m_entities() {

}

Scene::Scene(const std::vector<Entity *> &entities) {
    m_entities = entities;
}

void Scene::OnInitialize() const {
    for (Entity *entity : m_entities)
        entity->OnInitialize();
}

void Scene::OnUpdate(const Timestep timestep) const {
    for (Entity *entity : m_entities)
        entity->OnUpdate(timestep);
}

void Scene::OnRender() const {
    for (Entity *entity : m_entities)
        entity->OnRender();
}

void Scene::OnShutdown() const {
    for (Entity *entity : m_entities)
        entity->OnShutdown();
}

std::vector<Entity *> Scene::GetEntities() {
    return m_entities;
}