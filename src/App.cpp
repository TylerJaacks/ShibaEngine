#include "App.h"

#include "Core/Config.h"

App::App() {
    const UserConfig userConfig = Config::LoadUserConfig("Config/User.json");

    m_engine = new Engine{"ShibaEngine", userConfig};
}

App::~App() {
    delete m_engine;
    m_engine = nullptr;
}

void App::Run() const {
    m_engine->Run();
}