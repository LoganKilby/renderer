/* date = September 14th 2021 10:29 am */

#ifndef MATH_UTIL_H
#define MATH_UTIL_H

struct euler_angles
{
    float Pitch; // x-axis
    float Yaw;   // y-axis
    float Roll;  // z-axis
};

inline glm::mat4
mat4_EncodeEulerAnglesZYX(euler_angles Rotation)
{
    glm::mat4 RotationMatrix(1.0f);
    
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.Roll), glm::vec3(0, 0, 1));
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.Yaw), glm::vec3(0, 1, 0));
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.Pitch), glm::vec3(1, 0, 0));
    
    return RotationMatrix;
}

inline glm::vec3
ProjectVec3(glm::vec3 A, glm::vec3 B)
{
    return (B * (glm::dot(A, B) / glm::dot(B, B)));
}

#endif //MATH_UTIL_H
