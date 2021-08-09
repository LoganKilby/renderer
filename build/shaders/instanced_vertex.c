#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aInstanceMatrix;

// NOTE: OUT VARIABLES MUST HAVE CORRESPONDING VARIABLES IN THE FRAGMENT SHADER
//       BOTH OF THE VARIABLES NEED TO HAVE THE SAME NAME

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    Normal = aNormal;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
}