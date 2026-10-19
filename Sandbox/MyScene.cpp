#include <MyScene.h>

#include <MyEntity.h>

MyScene::~MyScene() = default;

void MyScene::OnInitialize() {
    m_entity = &CreateEntity<MyEntity>();

    // Called last so the entities created above get initialized too.
    // Update, Render and Shutdown are handled by Scene, which visits every entity once.
    Scene::OnInitialize();
}
