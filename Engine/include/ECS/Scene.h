#ifndef SHIBAENGINE_SCENE_H
#define SHIBAENGINE_SCENE_H

#include <Core/Timestep.h>
#include <ECS/Entity.h>

#include <memory>
#include <utility>
#include <vector>

class Scene {
public:
    Scene();

    virtual ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    virtual void OnInitialize();
    virtual void OnUpdate(Timestep timestep);
    virtual void OnRender();
    virtual void OnShutdown();

    // The scene owns every entity it creates; the returned reference stays valid for the scene's lifetime.
    template<typename T = Entity, typename... Args>
    T& CreateEntity(Args&&... args) {
        auto entity = std::make_unique<T>(std::forward<Args>(args)...);
        T& reference = *entity;

        m_entities.push_back(std::move(entity));

        return reference;
    }

    [[nodiscard]] const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return m_entities; }
private:
    std::vector<std::unique_ptr<Entity>> m_entities;
};

#endif //SHIBAENGINE_SCENE_H
