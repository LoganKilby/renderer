/* date = July 15th 2021 2:42 pm */

#ifndef OPENGL_CODE_H
#define OPENGL_CODE_H

internal void SetUniform3fv(int Program, char *Name, glm::vec3 Data);
internal void SetUniform1f(int Program, char *Name, float Data);

float TriangleVert1[] = 
{   // vertices         // color
    -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.5f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f
};

float TriangleVert2[] = 
{
    0, 0, 0,
    0.5f, 1, 0,
    1, 0, 0
};

float TriangleVert2TextureCoords[] = 
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.5f, 1.0f
};


float RectangleVertices[] =
{
    // positions         // colors          // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // top left 
};

unsigned int UniqueRectangleVertices[] =
{
    0, 1, 3, // first triangle (top right, bottom right, top left)
    1, 2, 3  // second triangle (bottom right, bottom left, top left)
};

struct opengl_buffer
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

enum shader_type 
{
    VERTEX,
    GEOMETRY,
    FRAGMENT,
    COMPUTE,
    TESS_CONTROL,
    TESS_EVALUATION
};

struct opengl_shader
{
    unsigned int Id;
    int Type;
};

struct opengl_shader_program
{
    unsigned int Id;
};

struct material
{
    unsigned int DiffuseMapTexUnit;
    unsigned int SpecularMapTexUnit;
    float Shininess;
};

struct directional_light
{
    glm::vec3 Direction;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};

struct point_light
{
    glm::vec3 Position;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    
    // Attenuation 
    float Constant;
    float Linear;
    float Quadratic;
};

struct spot_light
{
    glm::vec3 Position;
    glm::vec3 Direction;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    
    // Smoothing boundary
    float NearRadius;
    float FarRadius;
};

struct texture
{
    unsigned int TextureId;
    int Width;
    int Height;
    int ColorChannels;
    unsigned char *Data;
};

// Personal NOTE: OpenGL guarentees at least 16 4-component vertex attributes (vetex shader input
// variables) per shader. This can be tested with glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);


/*
float lightDistance = length(light.position - FragPos);
float lightAttenuation = 1 / (light.constant + (light.linear * lightDistance) + (light.quadratic * lightDistance * lightDistance));
*/

#endif //OPENGL_CODE_H
