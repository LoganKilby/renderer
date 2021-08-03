#version 330 core
layout (location = 0) in vec3 VertexIn;
layout (location = 1) in vec2 TexCoordsIn;

out vec2 TexCoords;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(VertexIn, 1.0);
    TexCoords = TexCoordsIn;
}