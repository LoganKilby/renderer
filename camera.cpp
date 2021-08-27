#include "camera.h"

internal void
RotateCamera(camera *Camera, float Yaw, float Pitch)
{
    Camera->Orientation.Pitch += Pitch;
    Camera->Orientation.Yaw += Yaw;
    
    if (Camera->Orientation.Pitch > 89.0f) // This could be checked in input function
        Camera->Orientation.Pitch = 89.0f;
    if (Camera->Orientation.Pitch < -89.0f)
        Camera->Orientation.Pitch = -89.0f;
    
    glm::vec3 CameraAngle;
    CameraAngle.x = cos(glm::radians(Camera->Orientation.Yaw)) * cos(glm::radians(Camera->Orientation.Pitch));
    CameraAngle.y = sin(glm::radians(Camera->Orientation.Pitch));
    CameraAngle.z = sin(glm::radians(Camera->Orientation.Yaw)) * cos(glm::radians(Camera->Orientation.Pitch));
    
    Camera->Front = glm::normalize(CameraAngle);
}

internal glm::mat4
GetViewMatrix(camera Camera)
{
    return glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
}

