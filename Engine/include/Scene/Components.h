#pragma once

#include "glm/glm.hpp"

namespace Engine {

    struct TagComponent {
        std::string tag;
        TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}

		operator std::string& () { return tag; }
		operator const std::string& () const { return   tag; }
    };

    struct TransformComponent {
        glm::mat4 Transform = glm::mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : Transform(transform) {}

        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }        
    };
} // namespace Engine
