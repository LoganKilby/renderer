/* date = August 30th 2021 1:05 pm */

#ifndef CAMERA_H
#define CAMERA_H

struct euler_angles
{
    float Pitch;
    float Yaw;
    float Roll;
};

enum camera_mode
{
    STATIC_MODE = 0,
    FLY_MODE
};

struct camera
{
    float LookSpeed;
    float PanSpeed;
    float FieldOfView;
    camera_mode Mode;
    euler_angles Orientation;
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
};

#endif //CAMERA_H
