#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 mvp;

void main()
{
    TexCoords = vec3(aPos.x, aPos.y, aPos.z);
    
    vec4 pos = mvp * vec4(aPos, 1.0);
    
    // NOTE: When the perspective divide happens, the z coordinate is guaranteed to be 1,
    // the maximum depth value. 
    // 
    gl_Position = pos.xyww;
}