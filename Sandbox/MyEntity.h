#ifndef SHIBAENGINE_MYENTITY_H
#define SHIBAENGINE_MYENTITY_H

#include <Core/Math/Vector.h>
#include <ECS/Entity.h>

class CameraComponent;
class MeshComponent;
class TransformComponent;

class MyEntity : public Entity {
public:
    ~MyEntity() override;

    void OnInitialize() override;

    void OnUpdate(Timestep timestep) override;

    void OnRender() override;

    void OnShutdown() override;

private:
    // Owned by Entity; these are just typed handles.
    CameraComponent* m_cameraComponent = nullptr;
    MeshComponent* m_meshComponent = nullptr;
    TransformComponent* m_transformComponent = nullptr;

    // Units per second.
    Vector3<float> m_velocity{1.0f, 0.0f, 0.0f};
    Vector3<float> m_scaleVelocity{0.1f, 0.1f, 0.1f};
};

#endif //SHIBAENGINE_MYENTITY_H
