#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "ExampleLayer.h"


class Sandbox : public Engine::Application {
public:
    Sandbox() : Application("Sandbox") {
        pushLayer(std::make_shared<ExampleLayer>());
        //pushOverlay(std::make_shared<Engine::ImGuiLayer>("ImGui Layer"));
    }
    ~Sandbox(){
        
    }
};

std::unique_ptr<Engine::Application> Engine::createApplication() {
    return std::make_unique<Sandbox>();
}
