/* date = July 15th 2021 2:42 pm */

#ifndef OPENGL_CODE_H
#define OPENGL_CODE_H

#if ASSERTIONS_ENABLED
#define AssertUniformLoc(Integer) if(Integer == -1) {*(int *)0 = 0;}
#else
#define AssertUniformLoc(Integer)
#endif

static void SetUniform3fv(int Program, char *Name, glm::vec3 Data);
static void SetUniform1f(int Program, char *Name, float Data);
static void SetUniform1i(int Program, char *Name, int Data);

struct opengl_buffer
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
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
    
    // Attenuation 
    float Constant;
    float Linear;
    float Quadratic;
};

// Personal NOTE: OpenGL guarentees at least 16 4-component vertex attributes (vetex shader input
// variables) per shader. This can be tested with glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);

#endif //OPENGL_CODE_H
