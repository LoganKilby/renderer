#version 330 core
layout (location = 0) in vec3 inputPos;
layout (location = 1) in vec3 inputColor;
layout (location = 2) in vec2 inputTexCoord;

out vec4 outputColor;
out vec2 outputTexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    outputColor = inputColor;
    outputTexCoord = inputTexCoord;
}
