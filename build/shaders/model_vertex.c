#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
// NOTE: OUT VARIABLES MUST HAVE CORRESPONDING VARIABLES IN THE FRAGMENT SHADER
//       BOTH OF THE VARIABLES NEED TO HAVE THE SAME NAME

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
}