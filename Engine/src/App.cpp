#include <App.h>

#include <Core/Config.h>

App::App(Game* game)
    : m_game(game) {
    const UserConfig userConfig = Config::LoadUserConfig("Config/User.json");

    m_engine = new Engine{*m_game, userConfig};
}

App::~App() {
    delete m_engine;
    m_engine = nullptr;

    delete m_game;
    m_game = nullptr;
}

void App::Run() const {
    m_engine->Run();
}