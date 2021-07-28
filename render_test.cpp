#include <fstream>
#include "renderer.cpp"

// Vertex Data provided by Scratchpixel. Output produced by Maya

const v3 verts[146] = { 
    {        0,    39.034,         0}, {  0.76212,    36.843,         0}, 
    {        3,    36.604,         0}, {        1,    35.604,         0}, 
    {   2.0162,    33.382,         0}, {        0,    34.541,         0}, 
    {  -2.0162,    33.382,         0}, {       -1,    35.604,         0}, 
    {       -3,    36.604,         0}, { -0.76212,    36.843,         0}, 
    {-0.040181,     34.31,         0}, {   3.2778,    30.464,         0}, 
    {-0.040181,    30.464,         0}, {-0.028749,    30.464,         0}, 
    {   3.2778,    30.464,         0}, {   1.2722,    29.197,         0}, 
    {   1.2722,    29.197,         0}, {-0.028703,    29.197,         0}, 
    {   1.2722,    29.197,         0}, {   5.2778,    25.398,         0}, 
    { -0.02865,    25.398,         0}, {   1.2722,    29.197,         0}, 
    {   5.2778,    25.398,         0}, {   3.3322,    24.099,         0}, 
    {-0.028683,    24.099,         0}, {   7.1957,    20.299,         0}, 
    { -0.02861,    20.299,         0}, {   5.2778,    19.065,         0}, 
    {-0.028663,    18.984,         0}, {   9.2778,    15.265,         0}, 
    {-0.028571,    15.185,         0}, {   9.2778,    15.265,         0}, 
    {   7.3772,    13.999,         0}, {-0.028625,    13.901,         0}, 
    {   9.2778,    15.265,         0}, {   12.278,    8.9323,         0}, 
    {-0.028771,    8.9742,         0}, {   12.278,    8.9323,         0}, 
    {   10.278,    7.6657,         0}, {-0.028592,    7.6552,         0}, 
    {   15.278,    2.5994,         0}, {-0.028775,    2.6077,         0}, 
    {   15.278,    2.5994,         0}, {   13.278,    1.3329,         0}, 
    {-0.028727,    1.2617,         0}, {   18.278,   -3.7334,         0}, 
    {   18.278,   -3.7334,         0}, {   2.2722,   -1.2003,         0}, 
    {-0.028727,   -1.3098,         0}, {   4.2722,        -5,         0}, 
    {   4.2722,        -5,         0}, {-0.028727,        -5,         0}, 
    {  -3.3582,    30.464,         0}, {  -3.3582,    30.464,         0}, 
    {  -1.3526,    29.197,         0}, {  -1.3526,    29.197,         0}, 
    {  -1.3526,    29.197,         0}, {  -5.3582,    25.398,         0}, 
    {  -1.3526,    29.197,         0}, {  -5.3582,    25.398,         0}, 
    {  -3.4126,    24.099,         0}, {   -7.276,    20.299,         0}, 
    {  -5.3582,    19.065,         0}, {  -9.3582,    15.265,         0}, 
    {  -9.3582,    15.265,         0}, {  -7.4575,    13.999,         0}, 
    {  -9.3582,    15.265,         0}, {  -12.358,    8.9323,         0}, 
    {  -12.358,    8.9323,         0}, {  -10.358,    7.6657,         0}, 
    {  -15.358,    2.5994,         0}, {  -15.358,    2.5994,         0}, 
    {  -13.358,    1.3329,         0}, {  -18.358,   -3.7334,         0}, 
    {  -18.358,   -3.7334,         0}, {  -2.3526,   -1.2003,         0}, 
    {  -4.3526,        -5,         0}, {  -4.3526,        -5,         0}, 
    {        0,     34.31,  0.040181}, {        0,    30.464,   -3.2778}, 
    {        0,    30.464,  0.040181}, {        0,    30.464,  0.028749}, 
    {        0,    30.464,   -3.2778}, {        0,    29.197,   -1.2722}, 
    {        0,    29.197,   -1.2722}, {        0,    29.197,  0.028703}, 
    {        0,    29.197,   -1.2722}, {        0,    25.398,   -5.2778}, 
    {        0,    25.398,   0.02865}, {        0,    29.197,   -1.2722}, 
    {        0,    25.398,   -5.2778}, {        0,    24.099,   -3.3322}, 
    {        0,    24.099,  0.028683}, {        0,    20.299,   -7.1957}, 
    {        0,    20.299,   0.02861}, {        0,    19.065,   -5.2778}, 
    {        0,    18.984,  0.028663}, {        0,    15.265,   -9.2778}, 
    {        0,    15.185,  0.028571}, {        0,    15.265,   -9.2778}, 
    {        0,    13.999,   -7.3772}, {        0,    13.901,  0.028625}, 
    {        0,    15.265,   -9.2778}, {        0,    8.9323,   -12.278}, 
    {        0,    8.9742,  0.028771}, {        0,    8.9323,   -12.278}, 
    {        0,    7.6657,   -10.278}, {        0,    7.6552,  0.028592}, 
    {        0,    2.5994,   -15.278}, {        0,    2.6077,  0.028775}, 
    {        0,    2.5994,   -15.278}, {        0,    1.3329,   -13.278}, 
    {        0,    1.2617,  0.028727}, {        0,   -3.7334,   -18.278}, 
    {        0,   -3.7334,   -18.278}, {        0,   -1.2003,   -2.2722}, 
    {        0,   -1.3098,  0.028727}, {        0,        -5,   -4.2722}, 
    {        0,        -5,   -4.2722}, {        0,        -5,  0.028727}, 
    {        0,    30.464,    3.3582}, {        0,    30.464,    3.3582}, 
    {        0,    29.197,    1.3526}, {        0,    29.197,    1.3526}, 
    {        0,    29.197,    1.3526}, {        0,    25.398,    5.3582}, 
    {        0,    29.197,    1.3526}, {        0,    25.398,    5.3582}, 
    {        0,    24.099,    3.4126}, {        0,    20.299,     7.276}, 
    {        0,    19.065,    5.3582}, {        0,    15.265,    9.3582}, 
    {        0,    15.265,    9.3582}, {        0,    13.999,    7.4575}, 
    {        0,    15.265,    9.3582}, {        0,    8.9323,    12.358}, 
    {        0,    8.9323,    12.358}, {        0,    7.6657,    10.358}, 
    {        0,    2.5994,    15.358}, {        0,    2.5994,    15.358}, 
    {        0,    1.3329,    13.358}, {        0,   -3.7334,    18.358}, 
    {        0,   -3.7334,    18.358}, {        0,   -1.2003,    2.3526}, 
    {        0,        -5,    4.3526}, {        0,        -5,    4.3526} 
}; 

float TriangleCount = 128;

uint32 tris[128 * 3] = { 
    8,   7,   9,   6,   5,   7,   4,   3,   5,   2,   1,   3,   0,   9,   1, 
    5,   3,   7,   7,   3,   9,   9,   3,   1,  10,  12,  11,  13,  15,  14, 
    15,  13,  16,  13,  17,  16,  18,  20,  19,  17,  20,  21,  20,  23,  22, 
    20,  24,  23,  23,  26,  25,  24,  26,  23,  26,  27,  25,  26,  28,  27, 
    27,  30,  29,  28,  30,  27,  30,  32,  31,  30,  33,  32,  27,  30,  34, 
    32,  36,  35,  33,  36,  32,  36,  38,  37,  36,  39,  38,  38,  41,  40, 
    39,  41,  38,  41,  43,  42,  41,  44,  43,  44,  45,  43,  44,  47,  46, 
    44,  48,  47,  48,  49,  47,  48,  51,  50,  10,  52,  12,  13,  53,  54, 
    55,  17,  54,  13,  54,  17,  56,  57,  20,  17,  58,  20,  20,  59,  60, 
    20,  60,  24,  60,  61,  26,  24,  60,  26,  26,  61,  62,  26,  62,  28, 
    62,  63,  30,  28,  62,  30,  30,  64,  65,  30,  65,  33,  62,  66,  30, 
    65,  67,  36,  33,  65,  36,  36,  68,  69,  36,  69,  39,  69,  70,  41, 
    39,  69,  41,  41,  71,  72,  41,  72,  44,  44,  72,  73,  44,  74,  75, 
    44,  75,  48,  48,  75,  76,  48,  77,  51,  78,  80,  79,  81,  83,  82, 
    83,  81,  84,  81,  85,  84,  86,  88,  87,  85,  88,  89,  88,  91,  90, 
    88,  92,  91,  91,  94,  93,  92,  94,  91,  94,  95,  93,  94,  96,  95, 
    95,  98,  97,  96,  98,  95,  98, 100,  99,  98, 101, 100,  95,  98, 102, 
    100, 104, 103, 101, 104, 100, 104, 106, 105, 104, 107, 106, 106, 109, 108, 
    107, 109, 106, 109, 111, 110, 109, 112, 111, 112, 113, 111, 112, 115, 114, 
    112, 116, 115, 116, 117, 115, 116, 119, 118,  78, 120,  80,  81, 121, 122, 
    123,  85, 122,  81, 122,  85, 124, 125,  88,  85, 126,  88,  88, 127, 128, 
    88, 128,  92, 128, 129,  94,  92, 128,  94,  94, 129, 130,  94, 130,  96, 
    130, 131,  98,  96, 130,  98,  98, 132, 133,  98, 133, 101, 130, 134,  98, 
    133, 135, 104, 101, 133, 104, 104, 136, 137, 104, 137, 107, 137, 138, 109, 
    107, 137, 109, 109, 139, 140, 109, 140, 112, 112, 140, 141, 112, 142, 143, 
    112, 143, 116, 116, 143, 144, 116, 145, 119 
};

i2 ComputePixelCoordinates(v3 WorldVertex, mat4 WorldToCamera,
                           float ViewportWidth, float ViewportHeight,
                           uint32 ImageWidth, uint32 ImageHeight)
{
    // Transforming vertex from world space to camera space
    v3 CameraVertex = point_matrix_multiply(WorldVertex, WorldToCamera);
    
    // Transforming from Camera Space to Screen Space
    float ViewportX = CameraVertex.x / -CameraVertex.z;
    float ViewportY = CameraVertex.y / -CameraVertex.z;
    
    // Transforming from Screen Space to Normalized Device Coordinates
    float NormalizedX = (ViewportX + ViewportWidth * 0.5) / ViewportWidth;
    float NormalizedY = (ViewportY + ViewportHeight * 0.5) / ViewportHeight;
    
    // Transforming from Normalized Device Coordinates to Raster Coordinates
    i2 RasterCoordinates;
    RasterCoordinates.X = ImageWidth * NormalizedX;
    RasterCoordinates.Y = ImageHeight * (1 - NormalizedY);
    
    return RasterCoordinates;
}

int main()
{
    // Camera to World space matrix provided by Maya
    mat4 CameraToWorld = new_matrix(0.871214, 0, -0.490904, 0, 
                                    -0.192902, 0.919559, -0.342346, 0, 
                                    0.451415, 0.392953, 0.801132, 0, 
                                    14.777467, 29.361945, 27.993464, 1);
    
    mat4 WorldToCamera = invert_matrix(CameraToWorld);
    
    // The Viewport Width and Height are arbitrary, but the value of 2 produces an image that captures
    // the entire image we want to render. Uniformly reducing or increasing the viewport dimensions would
    // have the effect of zooming in or zooming out from the image
    float ViewportWidth = 2;
    float ViewportHeight = 2;
    
    // Final image dimensions
    uint32 ImageWidth = 512;
    uint32 ImageHeight = 512;
    
    // Outputting final image to the scalable vector graphics format (viewable in a web browser)
    std::ofstream OutFile;
    OutFile.open("./proj.svg");
    OutFile << "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" height=\"512\" width=\"512\">" << std::endl;
    
    for(uint32 i = 0; i < TriangleCount; ++i)
    {
        v3 WorldVert0 = verts[tris[i * 3]];
        v3 WorldVert1 = verts[tris[i * 3 + 1]];
        v3 WorldVert2 = verts[tris[i * 3 + 2]];
        
        i2 Raster0 = ComputePixelCoordinates(WorldVert0, WorldToCamera, ViewportWidth, 
                                             ViewportHeight, ImageWidth, ImageHeight);
        
        i2 Raster1 = ComputePixelCoordinates(WorldVert1, WorldToCamera, ViewportWidth, 
                                             ViewportHeight, ImageWidth, ImageHeight);
        
        i2 Raster2 = ComputePixelCoordinates(WorldVert2, WorldToCamera, ViewportWidth, 
                                             ViewportHeight, ImageWidth, ImageHeight);
        OutFile << "<line x1=\"" << Raster0.X << "\" y1=\"" << Raster0.Y << "\" x2=\"" << Raster1.X << "\" y2=\"" << Raster1.Y << "\" style=\"stroke:rgb(0,0,0);stroke-width:1\" />\n"; 
        OutFile << "<line x1=\"" << Raster1.X << "\" y1=\"" << Raster1.Y << "\" x2=\"" << Raster2.X << "\" y2=\"" << Raster2.Y << "\" style=\"stroke:rgb(0,0,0);stroke-width:1\" />\n"; 
        OutFile << "<line x1=\"" << Raster2.X << "\" y1=\"" << Raster2.Y << "\" x2=\"" << Raster0.X << "\" y2=\"" << Raster0.Y << "\" style=\"stroke:rgb(0,0,0);stroke-width:1\" />\n"; 
    }
    
    OutFile << "</svg>\n";
    OutFile.close();
    
    return 0;
}