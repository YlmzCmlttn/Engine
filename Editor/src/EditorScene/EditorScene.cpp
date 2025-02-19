#include "EditorScene/EditorScene.h"



EditorScene::EditorScene()
    : Scene("Editor Scene")
{   
    m_EditorSceneCamera = CreateRef<EditorSceneCamera>();
}

EditorScene::~EditorScene()
{
}   

void EditorScene::onAttach()
{
    Scene::onAttach();   
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

void EditorScene::onViewportResize(uint32_t width, uint32_t height)
{
    m_EditorSceneCamera->setViewportSize(width, height);
}

glm::mat4 EditorScene::getCameraViewProjectionMatrix()
{
    return m_EditorSceneCamera->getViewProjectionMatrix();
}

glm::mat4 EditorScene::getCameraViewMatrix()
{
    return m_EditorSceneCamera->getViewMatrix();
}