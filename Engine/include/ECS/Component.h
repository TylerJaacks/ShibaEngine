#ifndef SHIBAENGINE_COMPONENT_H
#define SHIBAENGINE_COMPONENT_H

#include <cstdint>
#include <Core/Timestep.h>

using ComponentTypeId = std::uint64_t;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    Component(const Component&) = default;
    Component& operator=(const Component&) = default;

    Component(Component&&) = default;
    Component& operator=(Component&&) = default;

    virtual void OnInitialize() {}
    virtual void OnUpdate(Timestep /*timestep*/) {}
    virtual void OnRender() {}
    virtual void OnShutdown() {}

    [[nodiscard]] virtual ComponentTypeId GetTypeId() const = 0;
    [[nodiscard]] virtual const char* GetTypeName() const = 0;
};

#endif //SHIBAENGINE_COMPONENT_H
