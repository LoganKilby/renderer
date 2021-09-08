#version 330 core

// The source for the grid shader is a combintion of these two sources:
// https://github.com/martin-pr/possumwood/wiki/Infinite-ground-plane-using-GLSL-shaders
// http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/

// Vertices:

// -1.0f,  1.0f,  0.0f,   
// -1.0f, -1.0f,  0.0f,  
//  1.0f, -1.0f,  0.0f,   

// -1.0f,  1.0f,  0.0f,   
//  1.0f, -1.0f,  0.0f,   
//  1.0f,  1.0f,  0.0f

layout (location = 0) in vec3 aPos;

out vec3 nearPoint;
out vec3 farPoint;

uniform mat4 projection;
uniform mat4 view;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 proj)
{
    mat4 viewInverse = inverse(view);
    mat4 projInverse = inverse(proj);
    vec4 unprojectedPoint = viewInverse * projInverse * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}


void main() 
{
    vec3 p = aPos;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection).xyz;
    farPoint = UnprojectPoint(p.x, p.y, 1.0, view, projection).xyz;
    gl_Position = vec4(p, 1.0);
}