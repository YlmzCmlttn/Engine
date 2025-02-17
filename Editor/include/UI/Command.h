#pragma once

namespace Engine{
    class Command
    {
    public:
        virtual ~Command(){}
        virtual void execute() = 0;
        virtual void undo() = 0;
    };    
}