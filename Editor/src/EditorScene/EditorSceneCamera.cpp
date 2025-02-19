#include "EditorScene/EditorSceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

EditorSceneCamera::EditorSceneCamera(/* args */)
{
    setProjectionType(ProjectionType::PERSPECTIVE);
    setPerspectiveNearClip(0.1f);
    setPerspectiveFarClip(1000.0f);
    setPerspectiveVerticalFOV(glm::radians(m_Zoom));
    calculateForwardVector();
    calculateViewMatrix();
}

EditorSceneCamera::~EditorSceneCamera()
{
}

void EditorSceneCamera::onUpdate(Timestep ts)
{
    onKeyboardInput(ts);
    onMouseMovement();
}

void EditorSceneCamera::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
	dispatcher.dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(EditorSceneCamera::onMouseScrollEvent));
}

void EditorSceneCamera::calculateViewMatrix(){
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

void EditorSceneCamera::calculateForwardVector(){
    const glm::vec3 worldUp = glm::vec3(0,1,0);
    m_Forward = glm::normalize(glm::vec3(
        cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
        sin(glm::radians(m_Pitch)),
        sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
    ));

    m_Right = glm::normalize(glm::cross(m_Forward, worldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}

void EditorSceneCamera::onMouseMovement(){
    if(Input::isMouseButtonPressed(Mouse::ButtonRight)){
        glm::vec2 currentMousePosition{Input::getMouseX(),Input::getMouseY()};
        glm::vec2 delta = currentMousePosition - m_InitialMousePosition;
        m_InitialMousePosition = currentMousePosition;
        m_Yaw += delta.x * m_MouseSensitivity;
        m_Pitch += delta.y * m_MouseSensitivity;
        m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
        calculateForwardVector();
    }else{
        m_InitialMousePosition = {Input::getMouseX(),Input::getMouseY()};
    }
}

void EditorSceneCamera::onKeyboardInput(Timestep ts){
    float velocity = m_MoveSpeed * ts;
    if(Input::isKeyPressed(Key::W)){
        m_Position += m_Forward * velocity;
    }
    if(Input::isKeyPressed(Key::S)){
        m_Position -= m_Forward * velocity;
    }
    if(Input::isKeyPressed(Key::A)){
        m_Position -= m_Right * velocity;
    }
    if(Input::isKeyPressed(Key::D)){
        m_Position += m_Right * velocity;
    }
    if(Input::isKeyPressed(Key::Q))   
    {
        m_Position += m_Up * velocity;
    }
    if(Input::isKeyPressed(Key::E))
    {
        m_Position -= m_Up * velocity;
    }
    calculateViewMatrix();
}

bool EditorSceneCamera::onMouseScrollEvent(MouseScrolledEvent mouseScrolledEvent)
{
    m_Zoom -= mouseScrolledEvent.getYOffset() * m_ZoomSpeed;
    m_Zoom = std::max(m_Zoom, 0.0f);
    m_Zoom = std::min(m_Zoom, 45.0f);

    setPerspectiveVerticalFOV(glm::radians(m_Zoom));
    return true;
}