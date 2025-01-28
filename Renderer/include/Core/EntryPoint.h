#pragma once
extern std::unique_ptr<Engine::Application> Engine::createApplication();
int main(){
	auto app = Engine::createApplication();
	app->run();
}