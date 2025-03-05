#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"


//X11 implementation

// imgui_impl_x11.cpp
// A minimal ImGui platform backend for X11 using native Xlib.
// This implementation uses the new ImGui input API (io.AddKeyEvent, etc.) and does not use
// the legacy io.KeyMap or io.KeysDown arrays.
//
// Usage:
//   ImGui_ImplX11_Init(display, window);
//   ...
//   // In your event loop, forward X11 events:
//   while (XPending(display))
//   {
//       XEvent event;
//       XNextEvent(display, &event);
//       ImGui_ImplX11_ProcessEvent(&event);
//       // ... your engine event processing ...
//   }
//   ...
//   ImGui_ImplX11_NewFrame();
//   ImGui::NewFrame();
//   // Build your UI...
//   ImGui::Render();
//   ...
//   ImGui_ImplX11_Shutdown();

#include "imgui.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <chrono>

// Global variables to hold X11 context
static Display*         g_Display = nullptr;
static ::Window         g_Window  = 0;
static std::chrono::steady_clock::time_point g_PrevTime = std::chrono::steady_clock::now();

// A helper function to map X11 KeySyms to ImGuiKey enums.
static ImGuiKey X11KeyToImGuiKey(KeySym key)
{
    switch(key)
    {
    case XK_Tab:         return ImGuiKey_Tab;
    case XK_Left:        return ImGuiKey_LeftArrow;
    case XK_Right:       return ImGuiKey_RightArrow;
    case XK_Up:          return ImGuiKey_UpArrow;
    case XK_Down:        return ImGuiKey_DownArrow;
    case XK_Page_Up:     return ImGuiKey_PageUp;
    case XK_Page_Down:   return ImGuiKey_PageDown;
    case XK_Home:        return ImGuiKey_Home;
    case XK_End:         return ImGuiKey_End;
    case XK_Insert:      return ImGuiKey_Insert;
    case XK_Delete:      return ImGuiKey_Delete;
    case XK_BackSpace:   return ImGuiKey_Backspace;
    case XK_space:       return ImGuiKey_Space;
    case XK_Return:      return ImGuiKey_Enter;
    case XK_Escape:      return ImGuiKey_Escape;
    // Map letters (assuming lower-case keys; you might wish to handle upper-case too)
    case 'a':            return ImGuiKey_A;
    case 'c':            return ImGuiKey_C;
    case 'v':            return ImGuiKey_V;
    case 'x':            return ImGuiKey_X;
    case 'y':            return ImGuiKey_Y;
    case 'z':            return ImGuiKey_Z;
    default:             return ImGuiKey_None;
    }
}

bool ImGui_ImplX11_Init(Display* display, ::Window window)
{
    if (!display || !window)
        return false;

    g_Display = display;
    g_Window  = window;

    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName = "imgui_impl_x11";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;   // We can honor GetMouseCursor() values.
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;      // We can set the mouse position.

    // (No need to set io.KeyMap since the new input API is used.)

    g_PrevTime = std::chrono::steady_clock::now();
    return true;
}

void ImGui_ImplX11_Shutdown()
{
    g_Display = nullptr;
    g_Window  = 0;
}

void ImGui_ImplX11_NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();

    // Update display size (in case of window resizing)
    if (g_Display && g_Window)
    {
        XWindowAttributes wa;
        if (XGetWindowAttributes(g_Display, g_Window, &wa))
            io.DisplaySize = ImVec2((float)wa.width, (float)wa.height);
    }

    // Update delta time using std::chrono
    auto current_time = std::chrono::steady_clock::now();
    float delta_time = std::chrono::duration<float>(current_time - g_PrevTime).count();
    g_PrevTime = current_time;
    io.DeltaTime = delta_time > 0.0f ? delta_time : (1.0f / 60.0f);

    // Update mouse position using XQueryPointer
    if (g_Display && g_Window)
    {
        Window root_return, child_return;
        int root_x, root_y, win_x, win_y;
        unsigned int mask_return;
        if (XQueryPointer(g_Display, g_Window, &root_return, &child_return,
                          &root_x, &root_y, &win_x, &win_y, &mask_return))
        {
            io.AddMousePosEvent((float)win_x, (float)win_y);
        }
    }
}

// Process a single XEvent and update ImGui's input state.
// Call this function from your X11 event loop.
bool ImGui_ImplX11_ProcessEvent(const XEvent* event)
{
    ImGuiIO& io = ImGui::GetIO();
    switch (event->type)
    {
    case KeyPress:
    {
        KeySym key = XLookupKeysym(const_cast<XKeyEvent*>(&event->xkey), 0);
        ImGuiKey imgui_key = X11KeyToImGuiKey(key);
        if (imgui_key != ImGuiKey_None)
            io.AddKeyEvent(imgui_key, true);

        // For text input, convert the key press to a char (if any)
        {
            char buf[32];
            int len = XLookupString(const_cast<XKeyEvent*>(&event->xkey), buf, sizeof(buf), nullptr, nullptr);
            for (int i = 0; i < len; i++)
                io.AddInputCharacter(buf[i]);
        }

        // Update modifier keys
        io.AddKeyEvent(ImGuiKey_ModCtrl,  (event->xkey.state & ControlMask) != 0);
        io.AddKeyEvent(ImGuiKey_ModShift, (event->xkey.state & ShiftMask) != 0);
        io.AddKeyEvent(ImGuiKey_ModAlt,   (event->xkey.state & Mod1Mask) != 0);
        io.AddKeyEvent(ImGuiKey_ModSuper, (event->xkey.state & Mod4Mask) != 0);
        return true;
    }
    case KeyRelease:
    {
        KeySym key = XLookupKeysym(const_cast<XKeyEvent*>(&event->xkey), 0);
        ImGuiKey imgui_key = X11KeyToImGuiKey(key);
        if (imgui_key != ImGuiKey_None)
            io.AddKeyEvent(imgui_key, false);

        // Update modifier keys (they might still be pressed)
        io.AddKeyEvent(ImGuiKey_ModCtrl,  (event->xkey.state & ControlMask) != 0);
        io.AddKeyEvent(ImGuiKey_ModShift, (event->xkey.state & ShiftMask) != 0);
        io.AddKeyEvent(ImGuiKey_ModAlt,   (event->xkey.state & Mod1Mask) != 0);
        io.AddKeyEvent(ImGuiKey_ModSuper, (event->xkey.state & Mod4Mask) != 0);
        return true;
    }
    case ButtonPress:
    {
        if (event->xbutton.button == Button1) io.AddMouseButtonEvent(0, true);
        if (event->xbutton.button == Button2) io.AddMouseButtonEvent(2, true);
        if (event->xbutton.button == Button3) io.AddMouseButtonEvent(1, true);
        // Handle mouse wheel: Button4 (up) and Button5 (down)
        if (event->xbutton.button == Button4) io.AddMouseWheelEvent(0, 1);
        if (event->xbutton.button == Button5) io.AddMouseWheelEvent(0, -1);
        return true;
    }
    case ButtonRelease:
    {
        if (event->xbutton.button == Button1) io.AddMouseButtonEvent(0, false);
        if (event->xbutton.button == Button2) io.AddMouseButtonEvent(2, false);
        if (event->xbutton.button == Button3) io.AddMouseButtonEvent(1, false);
        return true;
    }
    case MotionNotify:
    {
        // Mouse position is updated in NewFrame via XQueryPointer,
        // but you could update it here if you prefer.
        return true;
    }
    default:
        break;
    }
    return false;
}
