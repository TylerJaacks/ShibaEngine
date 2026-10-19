#ifndef SHIBAENGINE_COMPONENT_H
#define SHIBAENGINE_COMPONENT_H

#include <cstdint>

using ComponentTypeId = std::uint64_t;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    Component(const Component&) = default;
    Component& operator=(const Component&) = default;

    Component(Component&&) = default;
    Component& operator=(Component&&) = default;

    [[nodiscard]] virtual ComponentTypeId GetTypeId() const = 0;
    [[nodiscard]] virtual const char* GetTypeName() const = 0;
};

#endif //SHIBAENGINE_COMPONENT_H
