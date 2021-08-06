#version 330 core

struct material
{
    sampler2D DiffuseMaps[40];
    sampler2D SpecularMaps[40];
    float Shininess;
};

in vec2 TexCoord;
out vec4 FragColor;

uniform material Materials;

void main()
{
    FragColor = texture(Materials.DiffuseMaps[0], TexCoord);
}