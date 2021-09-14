/* date = September 14th 2021 10:29 am */

#ifndef MATH_UTIL_H
#define MATH_UTIL_H

struct euler_angles
{
    float Pitch; // x-axis
    float Yaw;   // y-axis
    float Roll;  // z-axis
};

inline float 
Dot(glm::vec3 A, glm::vec3 B)
{
    glm::vec3 C = A * B;
}

inline glm::mat4
Mat4_EncodeEulerAnglesZYX(euler_angles Rotation)
{
    glm::mat4 RotationMatrix(1.0f);
    
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.Roll), glm::vec3(0, 0, 1));
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.Yaw), glm::vec3(0, 1, 0));
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.Pitch), glm::vec3(1, 0, 0));
    
    return RotationMatrix;
}

#endif //MATH_UTIL_H
