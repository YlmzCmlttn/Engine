#pragma once

#include "Command.h"
#include "Scene/Entity.h"

namespace Engine{
    class EntityCommand : public Command{
    public:
        EntityCommand(Entity entity):
            m_Entity(entity)
        {
        }
    protected:
        Entity m_Entity;
    };

    class EntityTagChangeCommand : public EntityCommand{
    public:
        EntityTagChangeCommand(Entity entity, const std::string& newTag):
            EntityCommand(entity),
            m_NewTag(newTag)
        {
            m_OldTag = m_Entity.getComponent<TagComponent>().tag;
        }
        void execute() override{
            m_Entity.getComponent<TagComponent>().tag = m_NewTag;
        }
        void undo() override{
            m_Entity.getComponent<TagComponent>().tag = m_OldTag;
        }
    private:
        std::string m_NewTag;
        std::string m_OldTag;
    };

    class EntityTransformChangeCommand : public EntityCommand{
    public:
        EntityTransformChangeCommand(Entity entity, const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale):
            EntityCommand(entity),
            m_Translation(translation),
            m_Rotation(rotation),
            m_Scale(scale)
        {
            m_OldTranslation = m_Entity.getComponent<TransformComponent>().localPosition;
            m_OldRotation = glm::eulerAngles(m_Entity.getComponent<TransformComponent>().localRotation);
            m_OldScale = m_Entity.getComponent<TransformComponent>().localScale;
        }
        void execute() override{
            Entity(m_Entity).setTransform(m_Translation, m_Rotation, m_Scale);
        }
        void undo() override{
            Entity(m_Entity).setTransform(m_OldTranslation, m_OldRotation, m_OldScale);
        }
    private:
        glm::vec3 m_Translation;
        glm::vec3 m_Rotation;
        glm::vec3 m_Scale;
        glm::vec3 m_OldTranslation;
        glm::vec3 m_OldRotation;
        glm::vec3 m_OldScale;
    };
}