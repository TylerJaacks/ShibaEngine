#include <ECS/Scene.h>

Scene::Scene() : m_entities() {

}

Scene::Scene(const std::vector<Entity *> &entities) {
    m_entities = entities;
}

void Scene::OnInitialize() {
    for (Entity *entity : m_entities)
        entity->OnInitialize();
}

void Scene::OnUpdate(const Timestep timestep) {
    for (Entity *entity : m_entities)
        entity->OnUpdate(timestep);
}

void Scene::OnRender() {
    for (Entity *entity : m_entities)
        entity->OnRender();
}

void Scene::OnShutdown() {
    for (Entity *entity : m_entities)
        entity->OnShutdown();
}