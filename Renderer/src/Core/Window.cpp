#include "Core/Window.h"
#include "Platform/GLFW/GLFWWindow.h"

namespace Engine {

    Scope<Window> Window::Create(const WindowProps& props)
	{	
		return CreateScope<GLFWWindow>(props);
	}
} // namespace Engine
