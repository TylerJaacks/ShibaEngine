#include <ECS/Components/TransformComponent.h>

TransformComponent::TransformComponent() :
    m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f),
    m_rotation(0.0f, 0.0f, 0.0f) {
}

ComponentTypeId TransformComponent::GetTypeId() const {
    return TypeId;
}

const char* TransformComponent::GetTypeName() const {
    return "TransformComponent";
}

void TransformComponent::SetPositionVector(const Vector3<float> &position) {
    m_position = position;
}

void TransformComponent::SetScaleVector(const Vector3<float> &scale) {
    m_scale = scale;
}

void TransformComponent::SetRotationVector(const Vector3<float> &rotation) {
    m_rotation = rotation;
}

const Vector3<float> & TransformComponent::GetPositionVector() const {
    return m_position;
}

const Vector3<float> & TransformComponent::GetScaleVector() const {
    return m_scale;
}

const Vector3<float> & TransformComponent::GetRotationVector() const {
    return m_rotation;
}