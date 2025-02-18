#include "Scene/Scene.h"
#include "EditorSceneCamera.h"
using namespace Engine;
class EditorScene : public Scene{
    public:
        EditorScene();
        virtual ~EditorScene();

        virtual void onUpdate(Timestep ts) override;
        virtual void onRender(Timestep ts) override;
        virtual void onEvent(Event& e) override;

    private:
        std::shared_ptr<EditorSceneCamera> m_EditorSceneCamera;
};