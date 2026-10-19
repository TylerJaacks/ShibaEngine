#include <ECS/Components/CameraComponent.h>

CameraComponent::CameraComponent()
    : m_camera(),
      m_isPrimary(true),
      m_hasFixedAspectRatio(false),
      m_fieldOfView(45.0f),
      m_aspectRatio(16.0f / 9.0f),
      m_nearClip(0.1f),
      m_farClip(1000.0f) {
    m_camera.SetPerspective(m_fieldOfView, m_aspectRatio, m_nearClip, m_farClip);
}

ComponentTypeId CameraComponent::GetTypeId() const {
    return TypeId;
}

const char* CameraComponent::GetTypeName() const {
    return "CameraComponent";
}

void CameraComponent::SetPerspective(const float fieldOfView, const float aspectRatio, const float nearClip, const float farClip) {
    m_fieldOfView = fieldOfView;
    m_aspectRatio = aspectRatio;
    m_nearClip = nearClip;
    m_farClip = farClip;
    m_camera.SetPerspective(m_fieldOfView, m_aspectRatio, m_nearClip, m_farClip);
}

void CameraComponent::SetPrimary(const bool primary) {
    m_isPrimary = primary;
}

void CameraComponent::SetFixedAspectRatio(const bool fixedAspectRatio) {
    m_hasFixedAspectRatio = fixedAspectRatio;
}

Camera& CameraComponent::GetCamera() {
    return m_camera;
}

const Camera& CameraComponent::GetCamera() const {
    return m_camera;
}

bool CameraComponent::IsPrimary() const {
    return m_isPrimary;
}

bool CameraComponent::HasFixedAspectRatio() const {
    return m_hasFixedAspectRatio;
}

float CameraComponent::GetFieldOfView() const {
    return m_fieldOfView;
}

float CameraComponent::GetAspectRatio() const {
    return m_aspectRatio;
}

float CameraComponent::GetNearClip() const {
    return m_nearClip;
}

float CameraComponent::GetFarClip() const {
    return m_farClip;
}