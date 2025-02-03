#pragma once
#include "Core.h"
#include "Log.h"

// The client (e.g., SandBoxApp.cpp) defines createApplication().
extern std::unique_ptr<Engine::Application> Engine::createApplication();

int main(){
	Engine::Log::Init();
	auto app = Engine::createApplication(); // createApplication returns a unique_ptr<Application>
	// Set the static instance; now it's owned solely by s_Instance.
	//Engine::Application::s_Instance = std::move(app);
	Engine::Application::Get().run();
}