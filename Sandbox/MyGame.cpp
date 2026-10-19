#include <MyGame.h>

#include <MyScene.h>

MyGame::MyGame()
    : Game("MyGame"),
      m_scene(std::make_unique<MyScene>()) {
}

void MyGame::OnInitialize() {
    m_scene->OnInitialize();
}

void MyGame::OnUpdate(const Timestep timestep) {
    m_scene->OnUpdate(timestep);
}

void MyGame::OnRender() {
    m_scene->OnRender();
}

void MyGame::OnEvent(Event& event) {

}

void MyGame::OnShutdown() {
    m_scene->OnShutdown();
}

Game* CreateGame() {
    return new MyGame();
}
