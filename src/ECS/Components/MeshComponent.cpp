#include "ECS/Components/MeshComponent.h"

MeshComponent::MeshComponent(const MeshHandle meshHandle, const MaterialHandle materialHandle)
    : m_meshHandle(meshHandle),
      m_materialHandle(materialHandle),
      m_meshPath(),
      m_materialPath(),
      m_visible(true),
      m_castShadows(true),
      m_receiveShadows(true) {
}

ComponentTypeId MeshComponent::GetTypeId() const {
    return TypeId;
}

const char * MeshComponent::GetTypeName() const {
    return "MeshComponent";
}

void MeshComponent::SetMeshHandle(MeshHandle meshHandle) {
    m_meshHandle = meshHandle;
}

void MeshComponent::SetMaterialHandle(MaterialHandle materialHandle) {
    m_materialHandle = materialHandle;
}

void MeshComponent::SetMeshPath(const std::string &meshPath) {
    m_meshPath = meshPath;
}

void MeshComponent::SetMaterialPath(const std::string &materialPath) {
    m_materialPath = materialPath;
}

void MeshComponent::SetVisible(bool visible) {
    m_visible = visible;
}

void MeshComponent::SetCastShadows(bool castShadows) {
    m_castShadows = castShadows;
}

void MeshComponent::SetReceiveShadows(bool receiveShadows) {
    m_receiveShadows = receiveShadows;
}

MeshHandle MeshComponent::GetMeshHandle() const {
    return m_meshHandle;
}

MaterialHandle MeshComponent::GetMaterialHandle() const {
    return m_materialHandle;
}

const std::string & MeshComponent::GetMeshPath() const {
    return m_meshPath;
}

const std::string & MeshComponent::GetMaterialPath() const {
    return m_materialPath;
}

bool MeshComponent::IsVisible() const {
    return m_visible;
}

bool MeshComponent::CastsShadows() const {
    return m_castShadows;
}

bool MeshComponent::ReceivesShadows() const {
    return m_receiveShadows;
}

bool MeshComponent::HasMesh() const {
    return m_meshHandle != 0 || !m_meshPath.empty();
}