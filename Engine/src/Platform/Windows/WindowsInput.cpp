#include "Platform/Windows/WindowsInput.h"
#include <Windows.h>
#include "Core/Application.h"

namespace Engine {

    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::isKeyPressedImpl(KeyCode keycode)
    {
        // GetAsyncKeyState returns a short with the high-order bit set if the key is down.
        return (GetAsyncKeyState(static_cast<int>(keycode)) & 0x8000) != 0;
    }

    bool WindowsInput::isKeyReleasedImpl(KeyCode keycode)
    {
        // If the high-order bit is not set, then the key is released.
        return (GetAsyncKeyState(static_cast<int>(keycode)) & 0x8000) == 0;
    }

    bool WindowsInput::isMouseButtonPressedImpl(MouseCode button)
    {
        return (GetAsyncKeyState(static_cast<int>(button)) & 0x8000) != 0;
    }

    bool WindowsInput::isMouseButtonReleasedImpl(MouseCode button)
    {
        return (GetAsyncKeyState(static_cast<int>(button)) & 0x8000) == 0;
    }

    float WindowsInput::getMouseXImpl()
    {
        // Retrieve the native window handle (HWND) from your application.
        HWND hwnd = static_cast<HWND>(Application::Get().getWindow().getNativeWindow());
        POINT point;
        GetCursorPos(&point);         // Get cursor position in screen coordinates.
        ScreenToClient(hwnd, &point); // Convert to window (client) coordinates.
        return static_cast<float>(point.x);
    }

    float WindowsInput::getMouseYImpl()
    {
        HWND hwnd = static_cast<HWND>(Application::Get().getWindow().getNativeWindow());
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(hwnd, &point);
        return static_cast<float>(point.y);
    }

} // namespace Engine
