#pragma once

#include "Command.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
namespace Engine{
    class EntityCommand : public Command{
    public:
        EntityCommand(Entity entity):
            m_ID(entity.getComponent<IdComponent>().id),
            m_Scene(entity.getScene())
        {
        }
        virtual ~EntityCommand() = default;
    protected:
        UUID m_ID;
        Ref<Scene> m_Scene;
        Entity getEntity(){
            return m_Scene->getEntityByUUID(m_ID);
        }
    };

    class EntityTagChangeCommand : public EntityCommand{
    public:
        EntityTagChangeCommand(Entity entity, const std::string& newTag):
            EntityCommand(entity),
            m_NewTag(newTag)
        {
            m_OldTag = getEntity().getComponent<TagComponent>().tag;
        }
        void execute() override{
            Entity(getEntity()).getComponent<TagComponent>().tag = m_NewTag;
        }
        void undo() override{
            Entity(getEntity()).getComponent<TagComponent>().tag = m_OldTag;
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
            m_OldTranslation = getEntity().getComponent<TransformComponent>().localPosition;
            m_OldRotation = getEntity().getComponent<TransformComponent>().localRotation;
            m_OldScale = getEntity().getComponent<TransformComponent>().localScale;
        }
        void execute() override{
            Entity(getEntity()).setTransform(m_Translation, m_Rotation, m_Scale);
        }
        void undo() override{
            Entity(getEntity()).setTransform(m_OldTranslation, m_OldRotation, m_OldScale);
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