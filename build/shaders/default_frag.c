#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    FragColor = vec4(texture(Texture, TexCoord).rgb, 1.0);
}
