#include <MyEntity.h>

#include <ECS/Components/CameraComponent.h>
#include <ECS/Components/MeshComponent.h>
#include <ECS/Components/TransformComponent.h>

MyEntity::~MyEntity() = default;

void MyEntity::OnInitialize() {
    m_cameraComponent = &AddComponent<CameraComponent>();
    m_transformComponent = &AddComponent<TransformComponent>();
    m_meshComponent = &AddComponent<MeshComponent>();

    m_transformComponent->SetPositionVector({1.0f, 1.0f, 1.0f});
    m_transformComponent->SetScaleVector({1.0f, 1.0f, 1.0f});

    // Called last so the components added above get initialized too.
    Entity::OnInitialize();
}

void MyEntity::OnUpdate(const Timestep timestep) {
    Entity::OnUpdate(timestep);

    const float deltaSeconds = timestep.GetSeconds();

    // Update the Position and Scale per-frame.
    m_transformComponent->SetPositionVector(m_transformComponent->GetPositionVector() + m_velocity * deltaSeconds);
    m_transformComponent->SetScaleVector(m_transformComponent->GetScaleVector() + m_scaleVelocity * deltaSeconds);
}

void MyEntity::OnRender() {
    Entity::OnRender();
}

void MyEntity::OnShutdown() {
    Entity::OnShutdown();
}