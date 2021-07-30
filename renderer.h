/* date = July 29th 2021 4:53 pm */

#ifndef RENDERER_H
#define RENDERER_H

enum texture_map_enum
{
    DIFFUSE_MAP,
    SPECULAR_MAP
};

struct vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoordinates;
};

struct texture_unit
{
    unsigned int Id;
    texture_map_enum Type;
};

struct mesh
{
    vertex *Vertices;
    int VertexCount;
    unsigned int *Indices;
    int IndexCount;
    texture_unit *TextureUnits; // max 16
    int TexUnitCount;
    
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
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

#endif //RENDERER_H
