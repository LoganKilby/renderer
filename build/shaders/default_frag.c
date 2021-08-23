#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

void main()
{
    FragColor = vec4(texture(diffuseMap, TexCoord).rgb, 1.0);
}
