/* date = July 15th 2021 2:42 pm */

#ifndef OPENGL_CODE_H
#define OPENGL_CODE_H

#if UNIFORM_ASSERTIONS_ENABLED
#define AssertUniformLoc(Integer) if(Integer == -1) {*(int *)0 = 0;}
#else
#define AssertUniformLoc(Integer)
#endif

static void SetUniform3fv(int Program, char *Name, glm::vec3 Data);
static void SetUniform1f(int Program, char *Name, float Data);
static void SetUniform1i(int Program, char *Name, int Data);

struct opengl_render_object
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

struct offscreen_buffer
{
    unsigned int FrameBuffer; // NOTE: Bind to this before drawing
    unsigned int ColorBuffer; // NOTE: Bind this texture to the default framebuffer after drawing
    unsigned int RenderBuffer;
    unsigned int VAO;
};

struct shadow_map
{
    unsigned int FrameBuffer;
    unsigned int DepthBuffer; // Texture
    int DepthBufferWidth;
    int DepthBufferHeight;
};

struct gl_viewport
{
    int PosX;
    int PosY;
    int Width;
    int Height;
};

// Personal NOTE: OpenGL guarentees at least 16 4-component vertex attributes (vetex shader input
// variables) per shader. This can be tested with glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);

#endif //OPENGL_CODE_H
