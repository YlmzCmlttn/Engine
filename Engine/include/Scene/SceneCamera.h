#include "Renderer/Camera.h"

namespace Engine
{

    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType
        {
            PERSPECTIVE = 0,
            ORTHOGRAPHIC = 1
        };

    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void setOrthographic(float size, float nearClip, float farClip);
        void setPerspective(float verticalFOV, float nearClip, float farClip);

        void setViewportSize(uint32_t width, uint32_t height);

        inline float getOrthographicSize() const { return m_OrthographicSize; }
        inline float getOrthographicNearClip() const { return m_OrthographicNear; }
        inline float getOrthographicFarClip() const { return m_OrthographicFar; }

        inline float getPerspectiveVerticalFOV() const { return m_PerspectiveVerticalFOV; }
        inline float getPerspectiveNearClip() const { return m_PerspectiveNear; }
        inline float getPerspectiveFarClip() const { return m_PerspectiveFar; }

        inline void setOrthographicSize(float size) { m_OrthographicSize = size; }
        inline void setOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; }
        inline void setOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; }

        inline void setPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveVerticalFOV = verticalFOV; }
        inline void setPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; }
        inline void setPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; }

        inline ProjectionType getProjectionType() const { return m_ProjectionType; }
        inline void setProjectionType(ProjectionType type) { m_ProjectionType = type; }

    private:
        ProjectionType m_ProjectionType = ProjectionType::PERSPECTIVE;
        float m_PerspectiveVerticalFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
    };
}