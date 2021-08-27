#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureBuffer;

void main()
{             
    FragColor = vec4(texture(textureBuffer, TexCoords).rgb, 1.0);
}