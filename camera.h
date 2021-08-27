/* date = August 27th 2021 1:41 pm */

#ifndef CAMERA_H
#define CAMERA_H

struct euler_angles
{
    float Yaw;
    float Pitch;
    float Roll;
};

struct camera
{
    euler_angles Orientation;
    float LookSpeed;
    float PanSpeed;
    float FieldOfView= 45.0f;
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif //CAMERA_H
