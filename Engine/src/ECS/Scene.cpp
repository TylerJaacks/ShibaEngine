#include <ECS/Scene.h>

Scene::Scene() : m_entities() {
}

Scene::~Scene() = default;

void Scene::OnInitialize() {
    for (const auto& entity : m_entities)
        entity->OnInitialize();
}

void Scene::OnUpdate(const Timestep timestep) {
    for (const auto& entity : m_entities)
        entity->OnUpdate(timestep);
}

void Scene::OnRender() {
    for (const auto& entity : m_entities)
        entity->OnRender();
}

void Scene::OnShutdown() {
    for (const auto& entity : m_entities)
        entity->OnShutdown();
}