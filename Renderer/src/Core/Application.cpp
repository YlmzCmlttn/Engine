#include "Core/Application.h"

namespace Engine {
    std::unique_ptr<Application> Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        std::cout << "Application (" << name << ") initialized." << std::endl;
    }

    Application::~Application() {
        std::cout << "Application destroyed." << std::endl;
    }

    void Application::run() {
        std::cout << "Application is running..." << std::endl;
    }
}
