#pragma once
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

        inline void setViewportSize(uint32_t width, uint32_t height) { m_ViewportWidth = width; m_ViewportHeight = height; calculateProjectionMatrix(); }

        inline uint32_t getViewportWidth() const { return m_ViewportWidth; }
        inline uint32_t getViewportHeight() const { return m_ViewportHeight; }

        inline float getOrthographicSize() const { return m_OrthographicSize; }
        inline float getOrthographicNearClip() const { return m_OrthographicNear; }
        inline float getOrthographicFarClip() const { return m_OrthographicFar; }

        inline float getPerspectiveVerticalFOV() const { return m_PerspectiveVerticalFOV; }
        inline float getPerspectiveNearClip() const { return m_PerspectiveNear; }
        inline float getPerspectiveFarClip() const { return m_PerspectiveFar; }

        inline void setOrthographicSize(float size) { m_OrthographicSize = size; calculateProjectionMatrix(); }
        inline void setOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; calculateProjectionMatrix(); }
        inline void setOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; calculateProjectionMatrix(); }

        inline void setPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveVerticalFOV = verticalFOV; calculateProjectionMatrix(); }
        inline void setPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; calculateProjectionMatrix(); }
        inline void setPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; calculateProjectionMatrix(); }

        inline ProjectionType getProjectionType() const { return m_ProjectionType; }
        inline void setProjectionType(ProjectionType type) { m_ProjectionType = type; calculateProjectionMatrix(); }

        void calculateProjectionMatrix();

    private:
        ProjectionType m_ProjectionType = ProjectionType::PERSPECTIVE;
        float m_PerspectiveVerticalFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
        uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;


        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
    };
}