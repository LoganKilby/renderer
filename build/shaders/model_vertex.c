#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
// NOTE: OUT VARIABLES MUST HAVE CORRESPONDING VARIABLES IN THE FRAGMENT SHADER
//       BOTH OF THE VARIABLES NEED TO HAVE THE SAME NAME
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    Normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = mvp * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}