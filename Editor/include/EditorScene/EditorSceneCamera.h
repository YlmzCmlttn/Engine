#pragma once
#include "Scene/SceneCamera.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Core/Timestep.h"


using namespace Engine;
class EditorSceneCamera : public SceneCamera
{
public:
    EditorSceneCamera(/* args */);
    ~EditorSceneCamera();

    void onUpdate(Timestep ts);
    void onEvent(Event& e);

    inline const glm::mat4& getViewMatrix()const{return m_ViewMatrix;}
    inline glm::mat4 getViewProjectionMatrix()const{return m_ProjectionMatrix * m_ViewMatrix;}

    inline const glm::vec3& getPosition()const{return m_Position;}
    
    

private:
    bool onMouseScrollEvent(MouseScrolledEvent mouseScrolledEvent);
    void onKeyboardInput(Timestep ts);
    void onMouseMovement();
    void calculateViewMatrix();
    void calculateForwardVector();

    float m_MoveSpeed = 5.0f;
    
    float m_MouseSensitivity = 0.1f;
    float m_ZoomSpeed = 0.1f;
    
    glm::vec2 m_InitialMousePosition;

    glm::mat4 m_ViewMatrix;
    glm::vec3 m_Position = glm::vec3(0.0f,0.0f,3.0f);
    glm::vec3 m_Forward;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_Zoom = 45.0f;

};