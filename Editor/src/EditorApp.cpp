#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "EditorLayer.h"


class Editor : public Engine::Application {
public:
    Editor() : Application("Editor") {
        pushLayer(std::make_shared<EditorLayer>());
        //pushOverlay(std::make_shared<Engine::ImGuiLayer>("ImGui Layer"));
    }
    ~Editor(){
        
    }
};

std::unique_ptr<Engine::Application> Engine::createApplication() {
    return std::make_unique<Editor>();
}
