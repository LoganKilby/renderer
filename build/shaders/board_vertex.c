#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// NOTE: OUTPUT VARIABLES MUST HAVE CORRESPONDING INPUT VARIABLES IN THE FRAGMENT SHADER.
//       BOTH OF THE VARIABLES NEED TO HAVE THE SAME NAME.

out vec3 VertexColor;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    VertexColor = aColor;
}