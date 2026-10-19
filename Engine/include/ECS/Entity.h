#ifndef SHIBAENGINE_ENTITY_H
#define SHIBAENGINE_ENTITY_H

#include <memory>
#include <utility>
#include <vector>

#include <Core/Timestep.h>
#include <ECS/Component.h>

class Entity {
public:
    Entity();

    virtual ~Entity();

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    virtual void OnInitialize();
    virtual void OnUpdate(Timestep timestep);
    virtual void OnRender();
    virtual void OnShutdown();

    // The entity owns every component it creates; the returned reference stays valid for the entity's lifetime.
    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T& reference = *component;

        m_components.push_back(std::move(component));

        return reference;
    }

    template<typename T>
    [[nodiscard]] T* GetComponent() const {
        for (const auto& component : m_components) {
            if (component->GetTypeId() == T::TypeId) {
                return static_cast<T*>(component.get());
            }
        }

        return nullptr;
    }

    template<typename T>
    [[nodiscard]] bool HasComponent() const { return GetComponent<T>() != nullptr; }

    [[nodiscard]] const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_components; }
private:
    std::vector<std::unique_ptr<Component>> m_components;
};

#endif //SHIBAENGINE_ENTITY_H
