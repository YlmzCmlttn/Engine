#include "Platform/GLFW/GLFWWindow.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Core/Application.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/WindowEvent.h"
#include "GLFW/glfw3.h"
#include <glad/gl.h>
namespace Engine {
    static bool  s_GLFWInitialized =false;

    static void GLFWErrorCallback(int error,const char* description){
        ENGINE_CORE_ERROR("GLFW Error ({0}) : {1}",error,description);
    }
    GLFWWindow::GLFWWindow(const WindowProps& props)
    {
        Init(props);
    } 

    GLFWWindow::~GLFWWindow()
    {
        Shutdown();
    }

    void GLFWWindow::Init(const WindowProps& props)
    {
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;

        ENGINE_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);
        if(!s_GLFWInitialized){
            int success = glfwInit();
            ENGINE_CORE_ASSERT(success, "Could not initialize GLFW!");
            s_GLFWInitialized = true;
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
        ENGINE_CORE_ASSERT(status, "Failed to initialize Glad!");
        glfwSetWindowUserPointer(m_Window, &m_Data);
        setVSync(true);

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            if (action == GLFW_PRESS) {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
            } else if (action == GLFW_RELEASE) {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
            } else if (action == GLFW_REPEAT) {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            if (action == GLFW_PRESS) {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
            } else if (action == GLFW_RELEASE) {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event(xOffset, yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event(xPos, yPos);
            data.EventCallback(event);
        });
    }

    void GLFWWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        s_GLFWInitialized = false;
    }

    void GLFWWindow::onUpdate()
    {
        std::cout<<"GLFWWindow::onUpdate"<<std::endl;
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
        std::cout<<"GLFWWindow::onUpdate end"<<std::endl;
    }
    
}