#pragma once

#include "Core/Input.h"

namespace Engine {

    class GLFWInput : public Input {
    protected:
        bool isKeyPressedImpl(KeyCode keycode) override;
        bool isMouseButtonPressedImpl(MouseCode button) override;
        bool isKeyReleasedImpl(KeyCode keycode) override;
        bool isMouseButtonReleasedImpl(MouseCode button) override;
        float getMouseXImpl() override;
        float getMouseYImpl() override;
    };

}