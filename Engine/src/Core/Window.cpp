#include "Core/Window.h"
#include "Platform/GLFW/GLFWWindow.h"

namespace Engine {

    Window* Window::Create(const WindowProps& props)
	{	
		return new GLFWWindow(props);
	}
} // namespace Engine
