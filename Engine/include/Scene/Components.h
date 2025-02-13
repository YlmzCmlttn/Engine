#pragma once
#include "Scene/SceneCamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <glm/gtx/matrix_decompose.hpp> 

namespace Engine {

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
    struct TransformComponent : public std::enable_shared_from_this<TransformComponent>, public ICloneableComponent {
        // --- Proxy for glm::vec3 ---
        struct Vec3Proxy : public glm::vec3 {
            // A pointer back to the owner (not part of the arithmetic data).
            TransformComponent* owner = nullptr;

            // Constructor simply forwards to glm::vec3 and stores the owner pointer.
            Vec3Proxy(TransformComponent* owner, const glm::vec3& init)
                : glm::vec3(init), owner(owner) {}

            // Overload operator= to mark the owner dirty when assigned.
            Vec3Proxy& operator=(const glm::vec3& newValue) {
                glm::vec3::operator=(newValue);
                if (owner) { 
                    owner->markDirty();
                }
                return *this;
            }
            // operator glm::vec3() const { return *this; }
            // operator glm::vec3&() { return *this; }
            // operator const glm::vec3&() const { return *this; }

            // We already inherit all needed arithmetic operators from glm::vec3.
        };

        // Proxy for glm::quat (similar approach)
        struct QuatProxy : public glm::quat {
            TransformComponent* owner = nullptr;
            QuatProxy(TransformComponent* owner, const glm::quat& init)
                : glm::quat(init), owner(owner) {}
            QuatProxy& operator=(const glm::quat& newValue) {
                glm::quat::operator=(newValue);
                if (owner) { 
                    owner->markDirty();
                }
                return *this;
            }

            // operator glm::quat() const { return *this; }
            // operator glm::quat&() { return *this; }
            // operator const glm::quat&() const { return *this; }
        };
    public:        // Cached world transform.
        // Dirty flag – true if this transform or its parent changed.
        

        // Default constructor sets proxies with 'this'.
        TransformComponent()
            : localPosition(this, glm::vec3(0.0f)),
              localRotation(this, glm::quat()),
              localScale(this, glm::vec3(1.0f)) {}

        // Local transform values using proxies.
        Vec3Proxy localPosition = Vec3Proxy(this, glm::vec3(0.0f));
        QuatProxy localRotation = QuatProxy(this, glm::quat());
        Vec3Proxy localScale = Vec3Proxy(this, glm::vec3(1.0f));

        bool isDirty = true;

        glm::mat4 worldTransform = glm::mat4(1.0f);

        TransformComponent* parent = nullptr;
        std::vector<TransformComponent*> children;

        // Mark this transform as dirty.
        void markDirty() {
            if (!isDirty) {
                isDirty = true;
                // Optionally, propagate the dirty flag to children.
                for (auto child : children) {
                    child->markDirty();
                }
            }
        }

        // Sets the parent using a reference.
        // The ECS is responsible for managing the lifetime of these components.
        void setParent(TransformComponent& newParent, bool keepWorldTransform = true) {            
            if (keepWorldTransform) {
                // Ensure transforms are up to date.
                updateTransform();       // Update this transform (global)
                newParent.updateTransform(); // Update parent's transform

                // Compute new local transform:
                // newLocal = inverse(newParent.worldTransform) * this->worldTransform
                glm::mat4 invParentWorld = glm::inverse(newParent.worldTransform);
                glm::mat4 newLocalMatrix = invParentWorld * worldTransform;

                // Decompose the new local matrix into translation, rotation, and scale.
                glm::vec3 newPos;
                glm::vec3 newScale;
                glm::vec3 skew;
                glm::vec4 perspective;
                glm::quat newRot;
                bool decomposed = glm::decompose(newLocalMatrix, newScale, newRot, newPos, skew, perspective);
                if (decomposed) {
                    localPosition = newPos;
                    localRotation = newRot;
                    localScale    = newScale;
                } else {
                    std::cerr << "Warning: Matrix decomposition failed during re-parenting.\n";
                }
            }
            removeParent();

            // Set the new parent.
            parent = &newParent;
            // Add this transform as a child of the new parent.
            newParent.children.push_back(this);
            markDirty();
        }

        // Removes the current parent, if any.
        void removeParent() {
            if (parent) {
                // Remove this transform from the parent's children list.
                auto& siblings = parent->children;
                siblings.erase(
                    std::remove(siblings.begin(), siblings.end(), this),
                    siblings.end()
                );
                parent = nullptr;
                markDirty();
            }
        }        
        // Recursively update the world transform.
        void updateTransform(const glm::mat4& parentWorld = glm::mat4(1.0f)) {
            if (isDirty) {
                // Compute the local transform.
                glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), localPosition)
                                        * glm::toMat4(localRotation)
                                        * glm::scale(glm::mat4(1.0f), localScale);
                // Update the world transform.
                worldTransform = parentWorld * localTransform;
                isDirty = false;
            }
            // Update children.
            for (auto child : children) {
                child->updateTransform(worldTransform);
            }
        }

        std::unique_ptr<ICloneableComponent> clone() const override {
            TransformComponent copy = *this;
            // Reset the hierarchy pointers in the duplicate.
            copy.parent = nullptr;
            copy.children.clear();
            // Optionally: mark as dirty.
            copy.isDirty = true;
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
