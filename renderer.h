/* date = July 29th 2021 4:53 pm */

#ifndef RENDERER_H
#define RENDERER_H

#define FAST_OBJ_IMPLEMENTATION
#include "include/fast_obj/fast_obj.h"
typedef fastObjMesh fast_obj_mesh;

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "include/stb/stb_image.h"
// NOTE: stbi_set_flip_vertically_on_load(true); OpenGL reads texture coordinates with the y (or v)
// coordinate reversed from how textures are generally created. I think in most cases, normal maps
// will need to be flipped vertically when loaded.

#include "include/assimp/Importer.hpp"
#include "include/assimp/scene.h"
#include "include/assimp/postprocess.h"

struct scene_attributes
{
    // Reinhard tone mapping: vec3 color = HDR_Color / (HDR_Color + vec3(1.0))
    // Exposure tone mapping: vec3 color = vec3(1.0) - exp(-HDR_Color * exposure); exp == e^x
    float ToneCurve;
    float Exposure;
};

enum texture_map_enum
{
    DIFFUSE_MAP = 1,
    SPECULAR_MAP,
    NORMAL_MAP
};

struct texture_unit
{
    unsigned int Id;
    texture_map_enum Type;
    int Width;
    int Height;
    int ColorChannels;
    char Path[256];
};

struct texture_cache
{
    texture_unit Textures[50];
    int Count;
    int Size;
};

struct vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoordinates;
    glm::vec3 Tangent;
};

struct cubemap_texture_paths
{
    char *Left;
    char *Right;
    char *Top;
    char *Bottom;
    char *Back;
    char *Front;
};

// NOTE: This is for implementing MSAA without the help of a window library
struct msaa_framebuffer
{
    unsigned int ScreenVAO;
    unsigned int FrameBuffer;
    unsigned int MSAA_ColorBuffer;
    unsigned int RenderBuffer;
    
    unsigned int ResolvedFrameBuffer;
    unsigned int DRAW_ColorBuffer;
};

// TODO: I don't really need the entire "texture unit" in the mesh, just the Id.
// I should store the texture unit in the texture cache and store the texture IDs 
// in the mesh.
struct mesh
{
    std::vector<texture_unit> Textures;
    int IndexCount;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

struct model
{
    std::vector<mesh> Meshes;
    char Directory[256] = {};
    int DirectoryStrLen;
};

/* Light Attenuation table
 
Distance	Constant	Linear	Quadratic
7	1.0	0.7	1.8
13	1.0	0.35	0.44
20	1.0	0.22	0.20
32	1.0	0.14	0.07
50	1.0	0.09	0.032
65	1.0	0.07	0.017
100	1.0	0.045	0.0075
160	1.0	0.027	0.0028
200	1.0	0.022	0.0019
325	1.0	0.014	0.0007
600	1.0	0.007	0.0002
3250	1.0	0.0014	0.000007

*/

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

#endif //RENDERER_H
