#pragma once
namespace Engine
{
    class Application {
    public:
        Application(const std::string& name = "Application");

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        
        inline static Application& Get(){return *s_Instance;}

        virtual void run();
        virtual ~Application();
    private:

        static std::unique_ptr<Application> s_Instance;        
    };

    std::unique_ptr<Application> createApplication();
} // namespace Engine
