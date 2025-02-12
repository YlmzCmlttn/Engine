#pragma once
#include "entt/entity/registry.hpp"
#include "Scene/Components.h"

namespace Engine {
    void UpdateTransforms(entt::registry& registry) {
        auto view = registry.view<TransformComponent>();
        for(auto entity : view) {
            auto &transform = view.get<TransformComponent>(entity);            
            if(transform.parent == nullptr){
                transform.updateTransform();
            }
        }
    }
}