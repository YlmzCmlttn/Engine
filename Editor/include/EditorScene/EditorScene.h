#pragma once
#include "Scene/Entity.h"
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
        inline const glm::mat4& getCameraViewMatrix(){return m_EditorSceneCamera->getViewMatrix();}
        inline const glm::mat4& getCameraProjectionMatrix(){return m_EditorSceneCamera->getProjectionMatrix(); }
        inline glm::mat4 getCameraViewProjectionMatrix(){return m_EditorSceneCamera->getViewProjectionMatrix();}

    private:
        std::shared_ptr<EditorSceneCamera> m_EditorSceneCamera;
        std::shared_ptr<Mesh> m_Mesh;
};