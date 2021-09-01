#include "camera.h"

internal void
RotateFreeCamera(camera *Camera, euler_angles Offset, float dt)
{
    Camera->Orientation.Pitch += Offset.Pitch * Camera->LookSpeed * dt;
    Camera->Orientation.Yaw += Offset.Yaw * Camera->LookSpeed * dt;
    
    if(Camera->Orientation.Pitch > 89.0f)
        Camera->Orientation.Pitch = 89.0f;
    if(Camera->Orientation.Pitch < -89.0f)
        Camera->Orientation.Pitch = -89.0f;
    
    glm::vec3 CameraAngle;
    CameraAngle.x = cos(glm::radians(Camera->Orientation.Yaw)) * cos(glm::radians(Camera->Orientation.Pitch));
    CameraAngle.y = sin(glm::radians(Camera->Orientation.Pitch));
    CameraAngle.z = sin(glm::radians(Camera->Orientation.Yaw)) * cos(glm::radians(Camera->Orientation.Pitch));
    
    Camera->Front = glm::normalize(CameraAngle);
}

// Temporary
internal void
MoveCameraByKey(camera *Camera, int KeyPressed, float dt)
{
    switch(KeyPressed)
    {
        case GLFW_KEY_W:
        {
            Camera->Position += Camera->Front * Camera->PanSpeed * dt;
        } break;
        
        case GLFW_KEY_A:
        {
            Camera->Position -= glm::normalize(glm::cross(Camera->Front, Camera->Up)) * 
                Camera->PanSpeed * dt;
        } break;
        
        case GLFW_KEY_S:
        {
            Camera->Position -= Camera->Front * Camera->PanSpeed * dt;
        } break;
        
        case GLFW_KEY_D:
        {
            Camera->Position += glm::normalize(glm::cross(Camera->Front, Camera->Up)) * 
                Camera->PanSpeed * dt;
        } break;
        
        case GLFW_KEY_Q:
        {
            Camera->Position.y += Camera->PanSpeed * dt;
        } break;
        
        case GLFW_KEY_E:
        {
            Camera->Position.y -= Camera->PanSpeed * dt;
        } break;
    }
}

internal glm::mat4
GetCameraViewMatrix(camera Camera)
{
    return glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
}
