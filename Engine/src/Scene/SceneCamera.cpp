#include "Scene/SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
    SceneCamera::SceneCamera(){}

    void SceneCamera::setOrthographic(float size, float nearClip, float farClip){
        m_ProjectionType = ProjectionType::ORTHOGRAPHIC;
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
        calculateProjectionMatrix();
    }

    void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip){
        m_ProjectionType = ProjectionType::PERSPECTIVE;
        m_PerspectiveVerticalFOV = verticalFOV;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;
        calculateProjectionMatrix();
    }

    void SceneCamera::calculateProjectionMatrix(){
        switch (m_ProjectionType)
        {
            case ProjectionType::ORTHOGRAPHIC:{
                float aspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
                float width = m_OrthographicSize * aspectRatio;
                float height = m_OrthographicSize;
                m_ProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f);
                break;
            }
            case ProjectionType::PERSPECTIVE:{
                m_ProjectionMatrix = glm::perspectiveFov(m_PerspectiveVerticalFOV, (float)m_ViewportWidth, (float)m_ViewportHeight, m_PerspectiveNear, m_PerspectiveFar);
                break;
            }
        }
    }
    
} // namespace 
