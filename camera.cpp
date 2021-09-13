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

internal camera
RotateCamera(camera Camera, euler_angles Offset, float dt)
{
    camera Result = Camera;
    
    Result.Orientation.Pitch += Offset.Pitch * Camera.LookSpeed * dt;
    Result.Orientation.Yaw += Offset.Yaw * Camera.LookSpeed * dt;
    
    if(Result.Orientation.Pitch > 89.0f)
        Result.Orientation.Pitch = 89.0f;
    if(Result.Orientation.Pitch < -89.0f)
        Result.Orientation.Pitch = -89.0f;
    
    glm::vec3 CameraAngle;
    CameraAngle.x = cos(glm::radians(Result.Orientation.Yaw)) * cos(glm::radians(Result.Orientation.Pitch));
    CameraAngle.y = sin(glm::radians(Result.Orientation.Pitch));
    CameraAngle.z = sin(glm::radians(Result.Orientation.Yaw)) * cos(glm::radians(Result.Orientation.Pitch));
    Result.Front = glm::normalize(CameraAngle);
    
    return Result;
}

internal camera
TranslateCamera(camera Camera, key_table *KeyTable, float dt)
{
    // TODO: I could probably think of a better way to move the camera instead of checking
    // all possible movement keys.
    
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
        DirectionCount = 1;
    }
    
    camera Result = Camera;
    float Speed = (Result.PanSpeed * dt) / DirectionCount;
    
    if(KeyTable->Keys[GLFW_KEY_W] == PRESSED || KeyTable->Keys[GLFW_KEY_W] == REPEAT)
    {
        Result.Position += Result.Front * Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_A] == PRESSED || KeyTable->Keys[GLFW_KEY_A] == REPEAT)
    {
        Result.Position -= glm::normalize(glm::cross(Result.Front, Result.Up)) * 
            Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_S] == PRESSED || KeyTable->Keys[GLFW_KEY_S] == REPEAT)
    {
        Result.Position -= Result.Front * Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_D] == PRESSED || KeyTable->Keys[GLFW_KEY_D] == REPEAT)
    {
        Result.Position += glm::normalize(glm::cross(Result.Front, Result.Up)) * 
            Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_Q] == PRESSED || KeyTable->Keys[GLFW_KEY_Q] == REPEAT)
    {
        Result.Position.y += Speed;
    }
    
    if(KeyTable->Keys[GLFW_KEY_E] == PRESSED || KeyTable->Keys[GLFW_KEY_E] == REPEAT)
    {
        Result.Position.y -= Speed;
    }
    
    return Result;
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


