#pragma once
#include "Scene/Scene.h"
#include "EditorSceneCamera.h"
#include "Renderer/Mesh.h"
using namespace Engine;
class EditorScene : public Scene{
    public:
        EditorScene();
        virtual ~EditorScene();

        virtual void onAttach() override;
        virtual void onUpdate(Timestep ts) override;
        virtual void onRender(Timestep ts) override;
        virtual void onEvent(Event& e) override;
        virtual void onViewportResize(uint32_t width, uint32_t height) override;
        virtual glm::mat4 getCameraViewProjectionMatrix();
        virtual glm::mat4 getCameraViewMatrix();

    private:
        std::shared_ptr<EditorSceneCamera> m_EditorSceneCamera;
        std::shared_ptr<Mesh> m_Mesh;
};