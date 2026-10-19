#include "ECS/Components/TagComponent.h"

TagComponent::TagComponent() : m_tag() {

}

ComponentTypeId TagComponent::GetTypeId() const {
    return TypeId;
}

const char* TagComponent::GetTypeName() const {
    return "TagComponent";
}

void TagComponent::SetTag(std::string tag) {
    m_tag = std::move(tag);
}

const std::string& TagComponent::GetTag() const {
    return m_tag;
}