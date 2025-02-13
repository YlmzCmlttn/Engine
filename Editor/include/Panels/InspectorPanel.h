#pragma once

#include "Scene/Entity.h"

class InspectorPanel{
public:
    InspectorPanel();
    ~InspectorPanel();

    inline void setEntity(Engine::Entity entity){m_Entity = entity;}
    void onImGuiRender();

private:
    Engine::Entity m_Entity;
};