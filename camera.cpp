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
    CameraAngle.x =cos(glm::radians(Camera->Orientation.Yaw)) * cos(glm::radians(Camera->Orientation.Pitch));
    CameraAngle.y = sin(glm::radians(Camera->Orientation.Pitch));
    CameraAngle.z =sin(glm::radians(Camera->Orientation.Yaw)) * cos(glm::radians(Camera->Orientation.Pitch));
    Camera->Front = glm::normalize(CameraAngle);
}

internal void
MoveCameraByKeyPressed(camera *Camera, key_table KeyTable, float dt)
{
    int KeyIndexesToCheck[] = 
    {
        GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E
    };
    
    int DirectionCount = 0;
    for(int i = 0; i < ArrayCount(KeyIndexesToCheck); ++i)
    {
        if(KeyTable.Keys[KeyIndexesToCheck[i]] == PRESSED)
        {
            DirectionCount++;
        }
    }
    
    if(!DirectionCount)
    {
        return;
    }
    
    float Velocity = (Camera->PanSpeed * dt) / DirectionCount;
    
    // TODO: Do better
    
    if(KeyTable.Keys[GLFW_KEY_W] == PRESSED)
    {
        Camera->Position += Camera->Front * Velocity;
    }
    
    if(KeyTable.Keys[GLFW_KEY_A] == PRESSED)
    {
        Camera->Position -= glm::normalize(glm::cross(Camera->Front, Camera->Up)) * 
            Velocity;
    }
    
    if(KeyTable.Keys[GLFW_KEY_S] == PRESSED)
    {
        Camera->Position -= Camera->Front * Velocity;
    }
    
    if(KeyTable.Keys[GLFW_KEY_D] == PRESSED)
    {
        Camera->Position += glm::normalize(glm::cross(Camera->Front, Camera->Up)) * 
            Velocity;
    }
    
    if(KeyTable.Keys[GLFW_KEY_Q] == PRESSED)
    {
        Camera->Position.y += Velocity;
    }
    
    if(KeyTable.Keys[GLFW_KEY_E] == PRESSED)
    {
        Camera->Position.y -= Velocity;
    }
    
}

internal glm::mat4
GetCameraViewMatrix(camera Camera)
{
    return glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
}
