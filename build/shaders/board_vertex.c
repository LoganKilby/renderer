#version 330 core
layout (location = 0) in vec3 aPos;

// NOTE: OUT VARIABLES MUST HAVE CORRESPONDING VARIABLES IN THE FRAGMENT SHADER
//       BOTH OF THE VARIABLES NEED TO HAVE THE SAME NAME

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}