#version 330 core

in vec3 VertexColor;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scannerImage;

void main()
{
    //FragColor = vec4(VertexColor, 1.0);
    FragColor = vec4(texture(scannerImage, TexCoords).rgb, 1.0);
}
