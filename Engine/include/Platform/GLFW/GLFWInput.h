#pragma once

#include "Core/Input.h"

namespace Engine {

    class GLFWInput : public Input {
    protected:
        bool isKeyPressedImpl(int keycode) override;
        bool isMouseButtonPressedImpl(int button) override;
        float getMouseXImpl() override;
        float getMouseYImpl() override;
    };

}