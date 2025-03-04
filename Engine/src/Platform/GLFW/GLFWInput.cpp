#include "Platform/GLFW/GLFWInput.h"
#include <GLFW/glfw3.h>
#include "Core/Application.h"

namespace Engine {

    //Input* Input::s_Instance = new GLFWInput();

    bool GLFWInput::isKeyPressedImpl(KeyCode keycode) {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool GLFWInput::isKeyReleasedImpl(KeyCode keycode) {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_RELEASE;
    }

    bool GLFWInput::isMouseButtonPressedImpl(MouseCode button) {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    bool GLFWInput::isMouseButtonReleasedImpl(MouseCode button) {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_RELEASE;
    }   

    float GLFWInput::getMouseXImpl() {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return static_cast<float>(xpos);
    }

    float GLFWInput::getMouseYImpl() {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return static_cast<float>(ypos);
    }
}