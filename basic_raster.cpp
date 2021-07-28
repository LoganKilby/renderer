#include "renderer.cpp"
#include "cow.h"
#include <algorithm>

inline float
EdgeFunction(v3 Vertex0, v3 Vertex1, v3 Point)
{
    return (Point.x - Vertex0.x) * (Vertex1.y - Vertex0.y) - 
        (Point.y - Vertex0.y) * (Vertex1.x - Vertex0.x);
}

inline void
SetPixel(win32_offscreen_buffer Buffer, int RowOffset, int ColumnOffset, 
         unsigned char Red, unsigned char Green, unsigned char Blue)
{
    uint8 *Row = (uint8 *)Buffer.Memory + (Buffer.Pitch * RowOffset);
    uint32 *Pixel = (uint32 *)Row + ColumnOffset;
    *Pixel = ((Red << 16) | (Green << 8) | Blue); 
}

inline void
SetZPixel(win32_offscreen_buffer Buffer, int RowOffset, int ColumnOffset, float Input)
{
    uint8 *Row = (uint8 *)Buffer.Memory + (Buffer.Pitch * RowOffset);
    uint32 *Pixel = (uint32 *)Row + ColumnOffset;
    *Pixel = (uint32)Input;
}

inline float
GetPixel(win32_offscreen_buffer Buffer, int RowOffset, int ColumnOffset)
{
    uint8 *Row = (uint8 *)Buffer.Memory + (Buffer.Pitch * RowOffset);
    uint32 *Pixel = (uint32 *)Row + ColumnOffset;
    return (float)*Pixel;
    
}

static const float inchToMm = 25.4; 
enum FitResolutionGate { kFill = 0, kOverscan };

void computeScreenCoordinates(float filmApertureWidth, 
                              float filmApertureHeight, 
                              uint32_t imageWidth, 
                              uint32_t imageHeight, 
                              FitResolutionGate fitFilm, 
                              float nearClippingPLane, 
                              float focalLength, 
                              float *top, float *bottom, float *left, float *right) 
{ 
    float filmAspectRatio = filmApertureWidth / filmApertureHeight; 
    float deviceAspectRatio = imageWidth / (float)imageHeight; 
    
    *top = ((filmApertureHeight * inchToMm / 2) / focalLength) * nearClippingPLane; 
    *right = ((filmApertureWidth * inchToMm / 2) / focalLength) * nearClippingPLane; 
    
    // field of view (horizontal)
    float fov = 2 * 180 / PI * atan((filmApertureWidth * inchToMm / 2) / focalLength); 
    
    float xscale = 1; 
    float yscale = 1; 
    
    switch (fitFilm) { 
        default: 
        case kFill: 
        if (filmAspectRatio > deviceAspectRatio) { 
            xscale = deviceAspectRatio / filmAspectRatio; 
        } 
        else { 
            yscale = filmAspectRatio / deviceAspectRatio; 
        } 
        break; 
        case kOverscan: 
        if (filmAspectRatio > deviceAspectRatio) { 
            yscale = filmAspectRatio / deviceAspectRatio; 
        } 
        else { 
            xscale = deviceAspectRatio / filmAspectRatio; 
        } 
        break; 
    } 
    
    *right *= xscale; 
    *top *= yscale; 
    
    *bottom = -(*top); 
    *left = -(*right); 
} 

internal void 
convertToRaster(v3 vertexWorld, 
                mat4 worldToCamera, 
                float l, 
                float r, 
                float t, 
                float b, 
                float distToNearPlane, 
                uint32_t imageWidth, 
                uint32_t imageHeight, 
                v3 *vertexRaster) 
{ 
    v3 vertexCamera = point_matrix_multiply(vertexWorld, worldToCamera); 
    
    float x = 3.0f;
    // convert to screen space
    v2 vertexScreen;
    vertexScreen.x = distToNearPlane * vertexCamera.x / -vertexCamera.z; 
    vertexScreen.y = distToNearPlane * vertexCamera.y / -vertexCamera.z; 
    
    // now convert point from screen space to NDC space (in range [-1,1])
    v2 vertexNDC; 
    vertexNDC.x = 2 * vertexScreen.x / (r - l) - (r + l) / (r - l); 
    vertexNDC.y = 2 * vertexScreen.y / (t - b) - (t + b) / (t - b); 
    
    // convert to raster space
    vertexRaster->x = (vertexNDC.x + 1) / 2 * imageWidth; 
    // in raster space y is down so invert direction
    vertexRaster->y = (1 - vertexNDC.y) / 2 * imageHeight; 
    vertexRaster->z = -vertexCamera.z; 
} 

float min3(float a, float b, float c) 
{
    float temp = b < c ? b : c;
    float result = a < temp ? a : temp;
    return result;
} 

float max3(float a, float b, float c) 
{ 
    float temp = b > c ? b : c;
    float result = a > temp ? a : temp;
    return result;
} 

float edgeFunction(v3 a, v3 b, v3 c) 
{ return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); } 


int floorFloat(float x) 
{
    return (int)x;
}

int intMin(int x, int y)
{
    return x < y ? x : y;
}

int intMax(int x, int y)
{
    return x > y ? x : y;
}

float floatMax(float a, float b)
{
    return a > b ? a : b;
}


const uint32_t imageWidth = 640; 
const uint32_t imageHeight = 480;
mat4 worldToCamera = new_matrix(0.707107,  -0.331295, 0.624695,   0, 
                                0,          0.883452, 0.468521,   0, 
                                -0.707107, -0.331295, 0.624695,   0, 
                                -1.63871,  -5.747777, -40.400412, 1);
const uint32_t ntris = 3156; 
const float nearClippingPlane = 1; 
const float farClippingPlane = 1000; 
float focalLength = 20; // in mm 
// 35mm Full Aperture in inches
float filmApertureWidth = 0.980; 
float filmApertureHeight = 0.735;


internal void
Render(win32_offscreen_buffer Buffer, win32_offscreen_buffer ZBuffer)
{
    int BitmapMemorySize = (ZBuffer.Width * ZBuffer.Height) * 4;
    memset(ZBuffer.Memory, farClippingPlane, BitmapMemorySize);
    
    mat4 cameraToWorld = invert_matrix(worldToCamera);
    
    // compute screen coordinates
    float t, b, l, r; 
    computeScreenCoordinates(filmApertureWidth, filmApertureHeight, 
                             imageWidth, imageHeight, 
                             kOverscan, 
                             nearClippingPlane, 
                             focalLength, 
                             &t, &b, &l, &r);
    
    for(uint32 i = 0; i < ntris; ++i)
    {
        const v3 vec0 = vertices[nvertices[i * 3]]; 
        const v3 vec1 = vertices[nvertices[i * 3 + 1]]; 
        const v3 vec2 = vertices[nvertices[i * 3 + 2]];
        
        v3 v0Raster, v1Raster, v2Raster; 
        convertToRaster(vec0, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, &v0Raster); 
        convertToRaster(vec1, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, &v1Raster); 
        convertToRaster(vec2, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, &v2Raster);
        
        v0Raster.z = 1 / v0Raster.z, 
        v1Raster.z = 1 / v1Raster.z, 
        v2Raster.z = 1 / v2Raster.z;
        
        v2 st0 = st[stindices[i * 3]]; 
        v2 st1 = st[stindices[i * 3 + 1]]; 
        v2 st2 = st[stindices[i * 3 + 2]];
        
        st0.x *= v0Raster.z, st0.y *= v0Raster.z;
        st1.x *= v1Raster.z, st1.y *= v1Raster.z;
        st2.x *= v2Raster.z, st2.y *= v2Raster.z;
        
        float xmin = min3(v0Raster.x, v1Raster.x, v2Raster.x); 
        float ymin = min3(v0Raster.y, v1Raster.y, v2Raster.y); 
        float xmax = max3(v0Raster.x, v1Raster.x, v2Raster.x); 
        float ymax = max3(v0Raster.y, v1Raster.y, v2Raster.y); 
        
        // the triangle is out of screen
        if (xmin > imageWidth - 1 || xmax < 0 || ymin > imageHeight - 1 || ymax < 0) continue; 
        
        // be careful xmin/xmax/ymin/ymax can be negative. Don't cast to uint32_t
        uint32_t x0 = intMax(int32_t(0), (int32_t)(floorFloat(xmin))); 
        uint32_t x1 = intMin(int32_t(imageWidth) - 1, (int32_t)(floorFloat(xmax))); 
        uint32_t y0 = intMax(int32_t(0), (int32_t)(std::floor(ymin))); 
        uint32_t y1 = intMin(int32_t(imageHeight) - 1, (int32_t)(floorFloat(ymax)));
        
        float area = edgeFunction(v0Raster, v1Raster, v2Raster); 
        
        for (uint32_t y = y0; y <= y1; ++y)
        {
            for (uint32_t x = x0; x <= x1; ++x)
            {
                v3 pixelSample = { x + 0.5f, y + 0.5f, 0 }; 
                float w0 = edgeFunction(v1Raster, v2Raster, pixelSample); 
                float w1 = edgeFunction(v2Raster, v0Raster, pixelSample); 
                float w2 = edgeFunction(v0Raster, v1Raster, pixelSample);
                float oneOverZ = v0Raster.z * w0 + v1Raster.z * w1 + v2Raster.z * w2; 
                float z = 1 / oneOverZ; 
                
                if (z < GetPixel(ZBuffer, x, y))
                {
                    SetZPixel(ZBuffer, x, y, z);
                    
                    v2 st;
                    st.x = st0.x * w0 + st1.x * w1 + st2.x * w2;
                    st.y = st0.y * w0 + st1.y * w1 + st2.y * w2;
                    
                    st.x *= z;
                    st.y *= z;
                    
                    v3 v0Cam = point_matrix_multiply(vec0, worldToCamera);
                    v3 v1Cam = point_matrix_multiply(vec1, worldToCamera); 
                    v3 v2Cam = point_matrix_multiply(vec2, worldToCamera);
                    
                    
                    
                    float px = (v0Cam.x/-v0Cam.z) * w0 + (v1Cam.x/-v1Cam.z) * w1 + (v2Cam.x/-v2Cam.z) * w2; 
                    float py = (v0Cam.y/-v0Cam.z) * w0 + (v1Cam.y/-v1Cam.z) * w1 + (v2Cam.y/-v2Cam.z) * w2; 
                    
                    v3 pt = {px * z, py * z, -z }; // pt is in camera space 
                    
                    v3 n = cross_product(v3_subtract(v1Cam, v0Cam), v3_subtract(v2Cam, v0Cam)); 
                    n = normalize_vector(n); 
                    v3 viewDirection;
                    viewDirection.x = -pt.x;
                    viewDirection.y = -pt.y;
                    viewDirection.z = -pt.z;
                    viewDirection = normalize_vector(viewDirection);
                    
                    float nDotView =  floatMax(0.f, dot_product(n, viewDirection)); 
                    
                    const int M = 10; 
                    float checker = (fmod(st.x * M, 1.0) > 0.5) ^ (fmod(st.y * M, 1.0) < 0.5); 
                    float c = 0.3 * (1 - checker) + 0.7 * checker; 
                    nDotView *= c;
                    SetPixel(Buffer, y, x, nDotView * 255, nDotView * 255, nDotView * 255);
                }
            }
        }
    }
}
