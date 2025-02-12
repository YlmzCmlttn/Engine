#pragma once
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"
namespace Engine {

	class Input
	{
	public:
		static bool isKeyPressed(KeyCode keycode) { return s_Instance->isKeyPressedImpl(keycode); }
		static bool isKeyReleased(KeyCode keycode) { return s_Instance->isKeyReleasedImpl(keycode); }

		inline static bool isMouseButtonPressed(MouseCode button) { return s_Instance->isMouseButtonPressedImpl(button); }
		inline static bool isMouseButtonReleased(MouseCode button) { return s_Instance->isMouseButtonReleasedImpl(button); }
		inline static float getMouseX() { return s_Instance->getMouseXImpl(); }
		inline static float getMouseY() { return s_Instance->getMouseYImpl(); }
	protected:
		virtual bool isKeyPressedImpl(KeyCode keycode) = 0;
		virtual bool isKeyReleasedImpl(KeyCode keycode) = 0;
		virtual bool isMouseButtonPressedImpl(MouseCode button) = 0;
		virtual bool isMouseButtonReleasedImpl(MouseCode button) = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};

}