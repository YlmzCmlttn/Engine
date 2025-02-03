#pragma once
#include "Event.h"
namespace Engine
{
    class MouseMovedEvent : public Event
    {
    private:        
        float m_MouseX,m_MouseY;
    public:
        MouseMovedEvent(float x,float y)
            :m_MouseX{x},m_MouseY{y}{}
        
        inline float getX()const{return m_MouseX;}
        inline float getY()const{return m_MouseY;}

        std::string toString()const override{
            std::stringstream ss;
            ss<<"MouseMovedEvent: "<<m_MouseX<<", "<<m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse|EventCategoryInput)
    };

    class MouseScrolledEvent : public Event
    {
    private:        
        float m_XOffset,m_YOffset;
    public:
        MouseScrolledEvent(float xOff,float yOff)
            :m_XOffset{xOff},m_YOffset{yOff}{}
        
        inline float getXOffset()const{return m_XOffset;}
        inline float getYOffset()const{return m_YOffset;}

        std::string toString()const override{
            std::stringstream ss;
            ss<<"MouseScrolledEvent: "<<getXOffset()<<", "<<getYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse|EventCategoryInput)
    };

    class MouseButtonEvent : public Event
    {
    protected:
        int m_Button;
        MouseButtonEvent(int button)
            :m_Button{button}{}
    public:
        
        inline int getMouseButton()const{return m_Button;}
        

        EVENT_CLASS_CATEGORY(EventCategoryMouse|EventCategoryInput)
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            :MouseButtonEvent{button}{}
        

        std::string toString()const override{
            std::stringstream ss;
            ss<<"MouseButtonPressedEvent: "<<m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            :MouseButtonEvent{button}{}
        

        std::string toString()const override{
            std::stringstream ss;
            ss<<"MouseButtonReleasedEvent: "<<m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

    
    
} // namespace Engine