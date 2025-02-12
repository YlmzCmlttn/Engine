#pragma once

#include <glm/glm.hpp>

namespace Engine {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projectionMatrix);
		virtual ~Camera() = default;

		inline const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
		inline glm::mat4& getProjectionMatrix() { return m_ProjectionMatrix; }
		inline void setProjectionMatrix(const glm::mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }
		
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};
}
