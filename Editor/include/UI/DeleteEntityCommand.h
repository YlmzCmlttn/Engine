#pragma once

#include "Command.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "EntityCommands.h"
namespace Engine{
    class DeleteEntityCommand : public EntityCommand{
    public:
        DeleteEntityCommand(Entity entity, bool keepChildren = false);
        virtual ~DeleteEntityCommand() = default;
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
        bool m_KeepChildren;
        std::queue<std::unique_ptr<DeleteEntityCommand>> m_DeleteQueue;
    };
}