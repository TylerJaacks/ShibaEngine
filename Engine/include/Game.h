#ifndef SHIBAENGINE_GAME_H
#define SHIBAENGINE_GAME_H

#include <Core/Timestep.h>
#include <Events/Event.h>

#include <string>
#include <utility>

#if defined(_WIN32)
    #define SHIBA_GAME_EXPORT extern "C" __declspec(dllexport)
#else
    #define SHIBA_GAME_EXPORT extern "C" __attribute__((visibility("default")))
#endif

class Game {
public:
    explicit Game(std::string name) : m_name(std::move(name)) {}
    virtual ~Game() = default;

    virtual void OnInitialize() = 0;
    virtual void OnUpdate(Timestep timestep) = 0;
    virtual void OnRender() = 0;
    virtual void OnEvent(Event& event) = 0;
    virtual void OnShutdown() = 0;

    [[nodiscard]] const std::string& GetName() const { return m_name; }

private:
    std::string m_name;
};

// Every game module exports this. The Loader resolves it by name at runtime.
inline constexpr const char* CreateGameSymbol = "CreateGame";
using CreateGameFn = Game* (*)();

SHIBA_GAME_EXPORT Game* CreateGame();

#endif //SHIBAENGINE_GAME_H
