#include "EditorScene/EditorScene.h"



EditorScene::EditorScene()
    : Scene("Editor Scene")
{   
    m_EditorSceneCamera = CreateRef<EditorSceneCamera>();
}

EditorScene::~EditorScene()
{
}   

void EditorScene::onUpdate(Timestep ts)
{
    m_EditorSceneCamera->onUpdate(ts);
    Scene::onUpdate(ts);
}

void EditorScene::onRender(Timestep ts)
{
    Scene::onRender(ts,m_EditorSceneCamera->getViewMatrix(),m_EditorSceneCamera->getViewProjectionMatrix());    
}

void EditorScene::onEvent(Event& e)
{
    m_EditorSceneCamera->onEvent(e);
    Scene::onEvent(e);
}

