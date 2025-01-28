#include "Core/Application.h"
#include "Core/EntryPoint.h"

class Sandbox : public Engine::Application {
public:
    Sandbox() : Application("Sandbox") {
        std::cout << "Sandbox application initialized." << std::endl;
    }
    ~Sandbox() override {
        std::cout << "Sandbox application destroyed." << std::endl;
    }
    void run() override {
        std::cout << "Sandbox application is running..." << std::endl;
    }
};

std::unique_ptr<Engine::Application> Engine::createApplication() {
    return std::make_unique<Sandbox>();
}
