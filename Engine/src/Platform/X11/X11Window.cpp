#include "Platform/X11/X11Window.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Core/Application.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/WindowEvent.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <GL/glx.h>
#include <cstring>
#include <ctime>

namespace Engine {
    Ref<Window> Window::Create(const WindowProps& props) {
        auto window = std::make_shared<X11Window>(props);
        window->init(props);
        return window;
    }
    // Helper: returns current time in seconds using std::clock
    float X11Window::getTime() const {
        return static_cast<float>(std::clock()) / CLOCKS_PER_SEC;
    }

    X11Window::X11Window(const WindowProps& props)
    {
        // Constructor body left empty – initialization is performed in init()
    }

    X11Window::~X11Window()
    {
        shutdown();
    }

    void X11Window::init(const WindowProps& props)
    {
        m_Data.title  = props.title;
        m_Data.width  = props.width;
        m_Data.height = props.height;

        ENGINE_CORE_INFO("Creating X11 window {0} ({1}, {2})", props.title, props.width, props.height);

        // Open X display
        m_Display = XOpenDisplay(nullptr);
        ENGINE_CORE_ASSERT(m_Display, "Failed to open X display!");

        int screen = DefaultScreen(m_Display);

        // Set desired visual attributes for GLX
        static int visualAttribs[] = {
            GLX_RGBA,
            GLX_DEPTH_SIZE, 24,
            GLX_DOUBLEBUFFER,
            None
        };

        m_VisualInfo = glXChooseVisual(m_Display, screen, visualAttribs);
        ENGINE_CORE_ASSERT(m_VisualInfo, "No appropriate visual found");

        // Create a colormap and set window attributes
        Colormap colormap = XCreateColormap(m_Display, RootWindow(m_Display, m_VisualInfo->screen), m_VisualInfo->visual, AllocNone);

        XSetWindowAttributes swa;
        swa.colormap   = colormap;
        swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                         ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                         StructureNotifyMask;

        // Create the native window
        m_Window = XCreateWindow(m_Display,
                                 RootWindow(m_Display, m_VisualInfo->screen),
                                 0, 0,
                                 props.width, props.height,
                                 0,
                                 m_VisualInfo->depth,
                                 InputOutput,
                                 m_VisualInfo->visual,
                                 CWColormap | CWEventMask,
                                 &swa);

        // Set window title
        XStoreName(m_Display, m_Window, props.title.c_str());

        // Support for window close via WM_DELETE_WINDOW protocol
        Atom wmDelete = XInternAtom(m_Display, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(m_Display, m_Window, &wmDelete, 1);

        // Show the window
        XMapWindow(m_Display, m_Window);
        m_Context = Context<WindowType::X11>::Create(shared_from_this());
        m_Context->init();
        // Create a GLX context and make it current
        //auto m_GLXContext = glXCreateContext(m_Display, m_VisualInfo, nullptr, GL_TRUE);
        //glXMakeCurrent(m_Display, m_Window, m_GLXContext);

        //XFree(m_VisualInfo); //Need for creting context.

        // Create our engine's rendering context for X11
        

        // Optionally set VSync. Here we try to use glXSwapIntervalEXT if available.
        setVSync(true);
    }

    void X11Window::shutdown()
    {
        XDestroyWindow(m_Display, m_Window);
        XCloseDisplay(m_Display);
    }

    void X11Window::processEvents()
    {
        // Process all pending X events.
        while (XPending(m_Display))
        {
            XEvent event;
            XNextEvent(m_Display, &event);
            switch (event.type)
            {
                case Expose:
                    // (Optional) Handle expose/redraw events.
                    break;
                case ConfigureNotify:
                {
                    // Window resize event
                    XConfigureEvent xce = event.xconfigure;
                    m_Data.width  = xce.width;
                    m_Data.height = xce.height;
                    WindowResizeEvent resizeEvent(xce.width, xce.height);
                    m_Data.EventCallback(resizeEvent);
                    break;
                }
                case ClientMessage:
                {
                    // Window close event (WM_DELETE_WINDOW)
                    Atom wmDelete = XInternAtom(m_Display, "WM_DELETE_WINDOW", False);
                    if ((Atom)event.xclient.data.l[0] == wmDelete)
                    {
                        WindowCloseEvent closeEvent;
                        m_Data.EventCallback(closeEvent);
                    }
                    break;
                }
                case KeyPress:
                {
                    // Note: For simplicity, we use the raw keycode.
                    KeyPressedEvent keyPressEvent(event.xkey.keycode, 0);
                    m_Data.EventCallback(keyPressEvent);
                    break;
                }
                case KeyRelease:
                {
                    KeyReleasedEvent keyReleaseEvent(event.xkey.keycode);
                    m_Data.EventCallback(keyReleaseEvent);
                    break;
                }
                case ButtonPress:
                {
                    // Handle mouse buttons and mouse wheel
                    if (event.xbutton.button == 4)  // Scroll up
                    {
                        MouseScrolledEvent scrollEvent(0, 1);
                        m_Data.EventCallback(scrollEvent);
                    }
                    else if (event.xbutton.button == 5) // Scroll down
                    {
                        MouseScrolledEvent scrollEvent(0, -1);
                        m_Data.EventCallback(scrollEvent);
                    }
                    else
                    {
                        MouseButtonPressedEvent mousePressEvent(event.xbutton.button);
                        m_Data.EventCallback(mousePressEvent);
                    }
                    break;
                }
                case ButtonRelease:
                {
                    if (event.xbutton.button != 4 && event.xbutton.button != 5)
                    {
                        MouseButtonReleasedEvent mouseReleaseEvent(event.xbutton.button);
                        m_Data.EventCallback(mouseReleaseEvent);
                    }
                    break;
                }
                case MotionNotify:
                {
                    MouseMovedEvent mouseMoveEvent(event.xmotion.x, event.xmotion.y);
                    m_Data.EventCallback(mouseMoveEvent);
                    break;
                }
                default:
                    break;
            }
        }
    }

    void X11Window::swapBuffers()
    {
        //glXSwapBuffers(m_Display, m_Window);
        m_Context->swapBuffers();
    }

    void X11Window::onUpdate()
    {
        // Additional per-frame logic if needed.
    }
} // namespace Engine
