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

internal void
RotateCamera(camera *Camera, euler_angles Offset, float dt)
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

internal void
MoveCameraByKeyPressed(camera *Camera, key_table *KeyTable, float dt)
{
    int KeyIndexesToCheck[] = 
    {
        GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E
    };
    
    int DirectionCount = 0;
    for(int i = 0; i < ArrayCount(KeyIndexesToCheck); ++i)
    {
        if(KeyTable->Keys[KeyIndexesToCheck[i]] == PRESSED ||
           KeyTable->Keys[KeyIndexesToCheck[i]] == REPEAT)
        {
            DirectionCount++;
        }
    }
    
    if(!DirectionCount)
    {
        return;
    }
    
    float Speed = (Camera->PanSpeed * dt) / DirectionCount;
    
    // TODO: Do better
    
    if(KeyTable->Keys[GLFW_KEY_W] == PRESSED || KeyTable->Keys[GLFW_KEY_W] == REPEAT)
    {
        Camera->Position += Camera->Front * Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_A] == PRESSED || KeyTable->Keys[GLFW_KEY_A] == REPEAT)
    {
        Camera->Position -= glm::normalize(glm::cross(Camera->Front, Camera->Up)) * 
            Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_S] == PRESSED || KeyTable->Keys[GLFW_KEY_S] == REPEAT)
    {
        Camera->Position -= Camera->Front * Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_D] == PRESSED || KeyTable->Keys[GLFW_KEY_D] == REPEAT)
    {
        Camera->Position += glm::normalize(glm::cross(Camera->Front, Camera->Up)) * 
            Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_Q] == PRESSED || KeyTable->Keys[GLFW_KEY_Q] == REPEAT)
    {
        Camera->Position.y += Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_E] == PRESSED || KeyTable->Keys[GLFW_KEY_E] == REPEAT)
    {
        Camera->Position.y -= Speed;
    }
}

internal void
ZoomCamera(camera *Camera, float YOffset)
{
    Camera->FieldOfView -= YOffset;
    if(Camera->FieldOfView < 1.0f)
        Camera->FieldOfView = 1;
    if(Camera->FieldOfView > 45.0f)
        Camera->FieldOfView = 45.0f;
}

internal glm::mat4
GetCameraViewMatrix(camera Camera)
{
    return glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
}


