#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

// NOTE: OUTPUT VARIABLES MUST HAVE CORRESPONDING INPUT VARIABLES IN THE FRAGMENT SHADER.
//       BOTH OF THE VARIABLES NEED TO HAVE THE SAME NAME.

out vec3 VertexColor;
out vec2 TexCoords;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    VertexColor = aColor;
    TexCoords = aTexCoords;
}