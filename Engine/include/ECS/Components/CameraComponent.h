#ifndef SHIBAENGINE_CAMERACOMPONENT_H
#define SHIBAENGINE_CAMERACOMPONENT_H

#include <ECS/Component.h>
#include <Renderer/Camera.h>

class CameraComponent final : public Component {
public:
    static constexpr ComponentTypeId TypeId = 2;

    CameraComponent();
    ~CameraComponent() override = default;

    CameraComponent(const CameraComponent&) = default;
    CameraComponent& operator=(const CameraComponent&) = default;

    CameraComponent(CameraComponent&&) = default;
    CameraComponent& operator=(CameraComponent&&) = default;

    [[nodiscard]] ComponentTypeId GetTypeId() const override;
    [[nodiscard]] const char* GetTypeName() const override;

    void SetPerspective(float fieldOfView, float aspectRatio, float nearClip, float farClip);

    void SetPrimary(bool primary);
    void SetFixedAspectRatio(bool fixedAspectRatio);

    [[nodiscard]] Camera& GetCamera();
    [[nodiscard]] const Camera& GetCamera() const;

    [[nodiscard]] bool IsPrimary() const;
    [[nodiscard]] bool HasFixedAspectRatio() const;

    [[nodiscard]] float GetFieldOfView() const;
    [[nodiscard]] float GetAspectRatio() const;
    [[nodiscard]] float GetNearClip() const;
    [[nodiscard]] float GetFarClip() const;

private:
    Camera m_camera;

    bool m_isPrimary = true;
    bool m_hasFixedAspectRatio = false;

    float m_fieldOfView = 45.0f;
    float m_aspectRatio = 16.0f / 9.0f;
    float m_nearClip = 0.1f;
    float m_farClip = 1000.0f;
};

#endif //SHIBAENGINE_CAMERACOMPONENT_H