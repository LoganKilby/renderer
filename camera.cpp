internal void
UpdateCamera(GLFWwindow *Window, camera *Camera, float dt)
{
    // TODO: Remove GLFWwindow and use my own input management system
    if(glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
        Camera->Position += Camera->Front * Camera->PanSpeed * dt;
    if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        Camera->Position -= Camera->Front * Camera->PanSpeed * dt;
    if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        Camera->Position -= glm::normalize(glm::cross(Camera->Front, Camera->Up)) * Camera->PanSpeed * dt;
    if(glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        Camera->Position += glm::normalize(glm::cross(Camera->Front, Camera->Up)) * Camera->PanSpeed * dt;
    if(glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
        Camera->Position.y += Camera->PanSpeed * dt;
    if(glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
        Camera->Position.y -= Camera->PanSpeed * dt;
    if(glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Camera->Position.y += Camera->PanSpeed * dt;
    
    glm::vec3 CameraAngle;
    CameraAngle.x =cos(glm::radians(Camera->Orientation.Yaw)) * cos(glm::radians(Camera->Orientation.Pitch));
    CameraAngle.y = sin(glm::radians(Camera->Orientation.Pitch));
    CameraAngle.z =sin(glm::radians(Camera->Orientation.Yaw)) * cos(glm::radians(Camera->Orientation.Pitch));
    Camera->Front = glm::normalize(CameraAngle);
}

// This is under the assumption that Camera.Front has been updated and normalized
// with the using the camera's Orientation angles
internal glm::mat4
GetViewMatrix(camera Camera)
{
    glm::mat4 Result = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
    return Result;
}