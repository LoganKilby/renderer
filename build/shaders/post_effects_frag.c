#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

float offset = 1.0 / 300.0; // What is this?

void main()
{
    vec2 offsets[9] = vec2[](vec2(-offset, offset),
                             vec2(0.0f,    offset),
                             vec2(offset,  offset),
                             vec2(-offset, 0.0f),
                             vec2(0.0f,    0.0f),
                             vec2(offset,  0.0f),
                             vec2(-offset, -offset),
                             vec2(0.0f,    -offset),
                             vec2(offset,  -offset));
    
    float sharpenKernel[9] = float[](-1, -1, -1,
                                     -1,  9, -1,
                                     -1, -1, -1);
    
    float edgeKernel[9] = float[](1,  1,  1,
                                  1,  -8, 1,
                                  1,  1,  1);
    
    float blurKernel[9] = float[](1.0 / 16, 2.0 / 16, 1.0 / 16,
                                  2.0 / 16, 4.0 / 16, 2.0 / 16,
                                  1.0 / 16, 2.0 / 16, 1.0 / 16);
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    
    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        color += sampleTex[i] * sharpenKernel[i];
    }
    
    //FragColor = vec4(color, 1.0);
    FragColor = texture(screenTexture, TexCoords);
}