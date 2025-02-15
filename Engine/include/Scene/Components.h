#pragma once
#include "Scene/SceneCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <glm/gtx/matrix_decompose.hpp> 
#include "entt/entity/registry.hpp"

namespace Engine {

    struct DirtyFlagComponent {};

    struct ICloneableComponent {
        virtual std::unique_ptr<ICloneableComponent> clone() const = 0;
        virtual ~ICloneableComponent() = default;
    };

    // struct IdComponent {
    //     UUID id;

    //     IdComponent() = default;
    //     IdComponent(const IdComponent&) = default;
    //     IdComponent(UUID id) : id(id) {}
    // };

    struct TagComponent : public ICloneableComponent {
        std::string tag;
        TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}

		operator std::string& () { return tag; }
		operator const std::string& () const { return   tag; }

        std::unique_ptr<ICloneableComponent> clone() const override {
            TagComponent* clone = new TagComponent();
            clone->tag = tag;
            return std::unique_ptr<ICloneableComponent>(clone);            
        }
    };

    struct RelationshipComponent{
        std::size_t children{0};
        Entity first = Entity();
        Entity prev = Entity();
        Entity next = Entity();
        Entity parent = Entity();
    };

    struct TransformComponent : public ICloneableComponent {            
        TransformComponent()
            : position(glm::vec3(0.0f)),
              rotation(glm::quat()),
              scale(glm::vec3(1.0f)),
              localPosition(glm::vec3(0.0f)),
              localRotation(glm::quat()),
              localScale(glm::vec3(1.0f))
        {
            globalTransform = glm::mat4(1.0f);
        }

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        glm::vec3 localPosition;
        glm::quat localRotation;
        glm::vec3 localScale;

        glm::mat4 globalTransform;
        glm::mat4 localTransform;


        void updateLocalTransform() {
            localTransform = glm::translate(glm::mat4(1.0f), localPosition) * glm::toMat4(localRotation) * glm::scale(glm::mat4(1.0f), localScale);
        }

        void updateGlobalTransform() {
            globalTransform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), scale);
        }

        glm::mat4 getLocalTransform() const {
            return localTransform;
        }

        glm::mat4 getGlobalTransform() const {
            return globalTransform;
        }

        std::unique_ptr<ICloneableComponent> clone() const override {
            TransformComponent copy = *this;
            return std::make_unique<TransformComponent>(copy);
        }
        
    };
    

    struct CameraComponent : public ICloneableComponent {
        SceneCamera Camera;
        bool primary = true;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        operator SceneCamera& () { return Camera; }
        operator const SceneCamera& () const { return Camera; }

        std::unique_ptr<ICloneableComponent> clone() const override {
            CameraComponent copy = *this;
            copy.Camera = Camera;
            copy.primary = false;
            return std::make_unique<CameraComponent>(copy);
        }
    };

    
} // namespace Engine
