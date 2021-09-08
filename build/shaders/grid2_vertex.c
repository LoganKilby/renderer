#version 330 core

layout (location = 0) in vec3 P;

uniform mat4 projection;
uniform mat4 view;

out vec3 vP;
out vec3 near;
out vec3 far;


vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 proj)
{
    mat4 viewInverse = inverse(view);
    mat4 projInverse = inverse(proj);
    vec4 unprojectedPoint = viewInverse * projInverse * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
    vP = P.xyz;
    gl_Position = vec4(P, 1.0);
    near = UnprojectPoint(P.x, P.y, 0.0, view, projection);
    far = UnprojectPoint(P.x, P.y, 1.0, view, projection);
}