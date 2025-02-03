#pragma once
#include "Event.h"

namespace Engine
{
    class WindowResizeEvent : public Event
    {
    private:
        unsigned int m_Width,m_Height;
    public:
        WindowResizeEvent(unsigned int width,unsigned int height)
            :m_Width(width),m_Height(height){}

        inline unsigned int getWidth()const{return m_Width;}
        inline unsigned int getHeight()const{return m_Height;}

        std::string toString()const override{
            std::stringstream ss;
            ss<<"WindowResizeEvent: "<<m_Width<<", "<<m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent(){}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

} // namespace Engine