#ifndef SHIBAENGINE_MESHCOMPONENT_H
#define SHIBAENGINE_MESHCOMPONENT_H

#include <ECS/Component.h>

#include <cstdint>
#include <string>

using MeshHandle = std::uint64_t;
using MaterialHandle = std::uint64_t;

class MeshComponent final : public Component {
public:
    static constexpr ComponentTypeId TypeId = 3;

    MeshComponent() = default;
    MeshComponent(MeshHandle meshHandle, MaterialHandle materialHandle = 0);

    ~MeshComponent() override = default;

    MeshComponent(const MeshComponent&) = default;
    MeshComponent& operator=(const MeshComponent&) = default;

    MeshComponent(MeshComponent&&) = default;
    MeshComponent& operator=(MeshComponent&&) = default;

    [[nodiscard]] ComponentTypeId GetTypeId() const override;
    [[nodiscard]] const char* GetTypeName() const override;

    void SetMeshHandle(MeshHandle meshHandle);
    void SetMaterialHandle(MaterialHandle materialHandle);

    void SetMeshPath(const std::string& meshPath);
    void SetMaterialPath(const std::string& materialPath);

    void SetVisible(bool visible);
    void SetCastShadows(bool castShadows);
    void SetReceiveShadows(bool receiveShadows);

    [[nodiscard]] MeshHandle GetMeshHandle() const;
    [[nodiscard]] MaterialHandle GetMaterialHandle() const;

    [[nodiscard]] const std::string& GetMeshPath() const;
    [[nodiscard]] const std::string& GetMaterialPath() const;

    [[nodiscard]] bool IsVisible() const;
    [[nodiscard]] bool CastsShadows() const;
    [[nodiscard]] bool ReceivesShadows() const;
    [[nodiscard]] bool HasMesh() const;

private:
    MeshHandle m_meshHandle = 0;
    MaterialHandle m_materialHandle = 0;

    std::string m_meshPath;
    std::string m_materialPath;

    bool m_visible = true;
    bool m_castShadows = true;
    bool m_receiveShadows = true;
};

#endif //SHIBAENGINE_MESHCOMPONENT_H