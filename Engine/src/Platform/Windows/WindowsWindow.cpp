#include "Platform/Windows/WindowsWindow.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Core/Application.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/WindowEvent.h"

#include <windowsx.h>

#include <imgui.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Engine {
    Ref<Window> Window::Create(const WindowProps& props) {
        auto window = std::make_shared<WindowsWindow>(props);
        window->init(props);
        return window;
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
    }

    WindowsWindow::~WindowsWindow()
    {
        shutdown();
    }

    void WindowsWindow::init(const WindowProps& props)
    {
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;
        m_Data.VSync = true;

        ENGINE_CORE_INFO("Creating Windows window {0} ({1}, {2})", props.title, props.width, props.height);

        // Retrieve the application instance handle.
        m_HInstance = GetModuleHandle(nullptr);

        // Register the window class.
        WNDCLASS wc = {};
        wc.lpfnWndProc   = WindowsWindow::WindowProc;
        wc.hInstance     = m_HInstance;
        wc.lpszClassName = "EngineWindowsWindowClass";
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

        if (!RegisterClass(&wc))
        {
            ENGINE_CORE_ERROR("Failed to register window class!");
            return;
        }

        DWORD dwStyle = WS_OVERLAPPEDWINDOW;
        RECT rect = { 0, 0, (LONG)props.width, (LONG)props.height };
        AdjustWindowRect(&rect, dwStyle, FALSE);

        // Create the window. Pass "this" pointer so it can be retrieved in WindowProc.
        m_Window = CreateWindowEx(
            0,
            wc.lpszClassName,
            m_Data.title.c_str(),
            dwStyle,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            m_HInstance,
            this
        );

        if (!m_Window)
        {
            ENGINE_CORE_ERROR("Failed to create Windows window!");
            return;
        }

        // Store the pointer to this instance in the window's user data.
        SetWindowLongPtr(m_Window, GWLP_USERDATA, (LONG_PTR)this);


        m_HDC = GetDC(m_Window);

        // Set pixel format
        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int pixelFormat = ChoosePixelFormat(m_HDC, &pfd);
        if (!SetPixelFormat(m_HDC, pixelFormat, &pfd)) {
            ENGINE_CORE_ERROR("Failed to set pixel format!");
            return;
        }

        // Create your rendering context (for example, an OpenGL context).
        m_Context = Context<WindowType::WINDOWS>::Create(shared_from_this());
        m_Context->init();


        // Display the window.
        ShowWindow(m_Window, SW_SHOW);
        // Optionally, configure VSync if supported by your context.
    }

    void WindowsWindow::shutdown()
    {
        if (m_Window)
        {
            DestroyWindow(m_Window);
            m_Window = nullptr;
        }
        UnregisterClass("EngineWindowsWindowClass", m_HInstance);
    }

    void WindowsWindow::processEvents()
    {
        MSG msg;
        while (PeekMessage(&msg, m_Window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void WindowsWindow::swapBuffers()
    {
        SwapBuffers(m_HDC);
    }

    void WindowsWindow::onUpdate()
    {
        // Implement any periodic update logic here if needed.
    }

    // The window procedure that processes messages sent to the window.
    LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        // Retrieve the pointer to the WindowsWindow instance.
        WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (!window)
            return DefWindowProc(hWnd, message, wParam, lParam);

        if (!window->m_Data.EventCallback) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
            return true;


        switch (message)
        {
            case WM_SIZE:
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                window->m_Data.width = width;
                window->m_Data.height = height;
                WindowResizeEvent event(width, height);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_CLOSE:
            {
                WindowCloseEvent event;
                window->m_Data.EventCallback(event);
                PostQuitMessage(0);
                break;
            }
            case WM_KEYDOWN:
            {
                bool repeat = (lParam & 0x40000000) != 0;
                KeyPressedEvent event((int)wParam, repeat ? 1 : 0);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_KEYUP:
            {
                KeyReleasedEvent event((int)wParam);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_CHAR:
            {
                KeyTypedEvent event((unsigned int)wParam);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_MOUSEMOVE:
            {
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);
                MouseMovedEvent event((float)xPos, (float)yPos);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_LBUTTONDOWN:
            {
                MouseButtonPressedEvent event(0);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_LBUTTONUP:
            {
                MouseButtonReleasedEvent event(0);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_RBUTTONDOWN:
            {
                MouseButtonPressedEvent event(1);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_RBUTTONUP:
            {
                MouseButtonReleasedEvent event(1);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_MBUTTONDOWN:
            {
                MouseButtonPressedEvent event(2);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_MBUTTONUP:
            {
                MouseButtonReleasedEvent event(2);
                window->m_Data.EventCallback(event);
                break;
            }
            case WM_MOUSEWHEEL:
            {
                int delta = GET_WHEEL_DELTA_WPARAM(wParam);
                MouseScrolledEvent event(0, (float)delta);
                window->m_Data.EventCallback(event);
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return 0;
    }

} // namespace Engine
