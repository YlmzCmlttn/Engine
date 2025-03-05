#pragma once
#include "Event.h"

namespace Engine
{
    class KeyEvent : public Event
    {
    protected:
        int m_KeyCode;
        KeyEvent(int keycode):m_KeyCode{keycode}{}

    public:
        inline int GetKeyCode()const{return m_KeyCode;}
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    };

    class KeyPressedEvent : public KeyEvent
    {
    private:
        int m_RepeatCount;
    public:
        KeyPressedEvent(int keycode,int repeatCount)
            :KeyEvent{keycode},m_RepeatCount{repeatCount}{}

        inline int GetRepeatCount()const{return m_RepeatCount;}

        std::string toString()const override{
            std::stringstream ss;
            ss<<"KeyPressedEvent: "<<m_KeyCode<<" ("<<m_RepeatCount<<" repeats)";
            return ss.str();
        }
        EVENT_CLASS_TYPE(KeyPressed)
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            :KeyEvent{keycode}{}

        std::string toString()const override{
            std::stringstream ss;
            ss<<"KeyReleasedEvent: "<<m_KeyCode;
            return ss.str();
        }
        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keycode)
            :KeyEvent{keycode}{}
        
        std::string toString()const override{
            std::stringstream ss;
            ss<<"KeyTypedEvent: "<<m_KeyCode;
            return ss.str();
        }
        EVENT_CLASS_TYPE(KeyTyped)
    };
    
} // namespace Engine