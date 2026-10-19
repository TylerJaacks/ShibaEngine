#include <MyScene.h>

MyScene::~MyScene() = default;

void MyScene::OnInitialize() {
    Scene::OnInitialize();

    m_Entity = new Entity();

    GetEntities().emplace_back(m_Entity);
}

void MyScene::OnUpdate(const Timestep timestep) {
    Scene::OnUpdate(timestep);

    for (const auto& entity : GetEntities()) {
        entity->OnUpdate(timestep);
    }
}

void MyScene::OnRender() {
    Scene::OnRender();

    for (const auto& entity : GetEntities()) {
        entity->OnRender();
    }
}

void MyScene::OnShutdown() {
    Scene::OnShutdown();

    for (const auto& entity : GetEntities()) {
        entity->OnShutdown();
    }
}
