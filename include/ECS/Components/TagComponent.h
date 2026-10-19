#ifndef SHIBAENGINE_TAGCOMPONENT_H
#define SHIBAENGINE_TAGCOMPONENT_H

#include <string>

#include "ECS/Component.h"

class TagComponent final : public Component {
public:
    static constexpr ComponentTypeId TypeId = 4;

    TagComponent();
    ~TagComponent() override = default;;

    TagComponent(const TagComponent&) = default;
    TagComponent& operator=(const TagComponent&) = default;

    TagComponent(TagComponent&&) = default;
    TagComponent& operator=(TagComponent&&) = default;

    [[nodiscard]] ComponentTypeId GetTypeId() const override;
    [[nodiscard]] const char* GetTypeName() const override;

    void SetTag(std::string tag);
    [[nodiscard]] const std::string& GetTag() const;
private:
    std::string m_tag;
};

#endif //SHIBAENGINE_TAGCOMPONENT_H