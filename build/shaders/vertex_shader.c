#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // NOTE: This model matrix was uniformly scaled. If it was scaled non-uniformly, the lighting
    // would be distorted as the surface normals calculated in the fragment shader would be wrong.
    // NOTE: simply passing the normal in clip space would have worked too,
    // Normal = aNormal;
    //Normal = vec3(model * vec4(aNormal, 0.0));
}