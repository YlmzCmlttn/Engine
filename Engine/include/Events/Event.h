#pragma once
#include "../Core/Core.h"


namespace Engine
{
    enum class EventType{
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick,AppUpdate,AppRender,
        KeyPressed,KeyRelease, KeyTyped,
        MouseButtonPressed,MouseButtonReleased,MouseMoved,MouseScrolled        
    };

    enum EventCategory{
        Nono=0,
        EventCategoryApplication    = BIT(0),
        EventCategoryWindow         = BIT(1),
        EventCategoryInput          = BIT(2),
        EventCategoryKeyboard       = BIT(3),
        EventCategoryMouse          = BIT(4),
        EventCategoryMouseButton    = BIT(5)

    };

#define EVENT_CLASS_TYPE(type) static EventType getStaticType(){return EventType::type;}\
                                virtual EventType getEventType()const override{return getStaticType();}\
                                virtual const char* getName()const override{return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags()const override{return category;}

    class Event
    {
    private:
        friend class EventDispatcher;
    public:
        bool m_Handled = false;
        virtual EventType getEventType()const = 0;
        virtual const char* getName()const = 0;
        virtual int getCategoryFlags()const = 0;
        virtual std::string toString()const{return getName();}
        inline bool isHandled()const{return m_Handled;}
        inline bool isInCategory(EventCategory category){
            return getCategoryFlags()&category;
        }
    protected:
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    private:
        Event& m_Event;
    public:
        EventDispatcher(Event& event)
            :m_Event{event}{}

        template<typename T>
        bool dispatch(EventFn<T> func){
            if(m_Event.getEventType()==T::getStaticType()){
                m_Event.m_Handled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e){
        return os<<e.toString();
    }    
} // namespace Engineenderer
