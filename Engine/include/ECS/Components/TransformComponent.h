#ifndef SHIBAENGINE_TRANSFORMCOMPONENT_H
#define SHIBAENGINE_TRANSFORMCOMPONENT_H

#include <ECS/Component.h>
#include <Core/Math/Vector.h>

class TransformComponent final : public Component {
public:
    static constexpr ComponentTypeId TypeId = 5;

    TransformComponent();
    ~TransformComponent() override = default;

    TransformComponent(const TransformComponent&) = default;
    TransformComponent& operator=(const TransformComponent&) = default;

    TransformComponent(TransformComponent&&) = default;
    TransformComponent& operator=(TransformComponent&&) = default;

    [[nodiscard]] ComponentTypeId GetTypeId() const override;
    [[nodiscard]] const char* GetTypeName() const override;

    void SetPositionVector(const Vector3<float> &position);
    void SetScaleVector(const Vector3<float> &scale);
    void SetRotationVector(const Vector3<float> &rotation);

    [[nodiscard]] const Vector3<float> &GetPositionVector() const;
    [[nodiscard]] const Vector3<float> &GetScaleVector() const;
    [[nodiscard]] const Vector3<float> &GetRotationVector() const;
private:
    Vector3<float> m_position;
    Vector3<float> m_scale;
    Vector3<float> m_rotation;
};

#endif //SHIBAENGINE_TRANSFORMCOMPONENT_H