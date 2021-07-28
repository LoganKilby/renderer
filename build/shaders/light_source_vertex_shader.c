#version 330 core
layout (location = 0) in vec3 vertexData;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(vertexData, 1.0);
}