#pragma once

#include "Command.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
namespace Engine{
    class DeleteEntityCommand : public Command{
    public:
        DeleteEntityCommand(Entity entity, bool keepChildren = false);
        void execute() override;
        void undo() override;
    private:
        struct RestoreRelationshipComponent{
            std::optional<UUID> parent = std::nullopt;
            std::optional<UUID> next = std::nullopt;
            std::optional<UUID> prev = std::nullopt;
            std::optional<UUID> first = std::nullopt;
        };
        RestoreRelationshipComponent m_RestoreRelationshipComponent;
        std::vector<std::function<void(Entity)>> restoreComponents;
        Ref<Scene> m_Scene;
        Entity m_Entity;
        bool m_KeepChildren;
    };
}