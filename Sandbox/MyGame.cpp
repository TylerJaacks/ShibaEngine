#include <MyGame.h>

#include "MyScene.h"

MyGame::MyGame() : Game("MyGame") {
    m_Scene = new MyScene();
}

void MyGame::OnInitialize() {
    m_Scene->OnInitialize();
}

void MyGame::OnUpdate(const Timestep timestep) {
    m_Scene->OnUpdate(timestep);
}

void MyGame::OnRender() {
    m_Scene->OnRender();
}

void MyGame::OnEvent(Event& /*event*/) {
}

void MyGame::OnShutdown() {
    m_Scene->OnShutdown();
}

Game* CreateGame() {
    return new MyGame();
}
