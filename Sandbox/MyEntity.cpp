#include <MyEntity.h>

#include <ECS/Components/CameraComponent.h>
#include <ECS/Components/MeshComponent.h>
#include <ECS/Components/TransformComponent.h>

MyEntity::~MyEntity() = default;

void MyEntity::OnInitialize() {
    Entity::OnInitialize();

    m_cameraComponent = new CameraComponent();
    m_transformComponent = new TransformComponent();
    m_meshComponent = new MeshComponent();

    GetComponents().emplace_back(m_cameraComponent);
    GetComponents().emplace_back(m_transformComponent);
    GetComponents().emplace_back(m_meshComponent);
}

void MyEntity::OnUpdate(const Timestep timestep) {
    Entity::OnUpdate(timestep);

    static_cast<TransformComponent *>(m_transformComponent)->SetPositionVector({
        static_cast<TransformComponent *>(m_transformComponent)->GetPositionVector().x * timestep.GetMilliseconds(),
        static_cast<TransformComponent *>(m_transformComponent)->GetPositionVector().y * timestep.GetMilliseconds(),
        static_cast<TransformComponent *>(m_transformComponent)->GetPositionVector().z * timestep.GetMilliseconds()
    });

    static_cast<TransformComponent *>(m_transformComponent)->SetScaleVector({
        static_cast<TransformComponent *>(m_transformComponent)->GetScaleVector().x * timestep.GetMilliseconds(),
        static_cast<TransformComponent *>(m_transformComponent)->GetScaleVector().y * timestep.GetMilliseconds(),
        static_cast<TransformComponent *>(m_transformComponent)->GetScaleVector().z * timestep.GetMilliseconds()
    });
}

void MyEntity::OnRender() {
    Entity::OnRender();
}

void MyEntity::OnShutdown() {
    Entity::OnShutdown();
}