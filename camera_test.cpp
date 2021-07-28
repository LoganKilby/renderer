#include <fstream>
#include "renderer.cpp"

// Camera Test: Rending an image of a boat through a pinhole camera
// Triangles that are outisde of the canvas region will be colored red

v3 Vertices[146] = { 
    {  -2.5703,   0.78053,  -2.4e-05}, { -0.89264,  0.022582,  0.018577}, 
    {   1.6878, -0.017131,  0.022032}, {   3.4659,  0.025667,  0.018577}, 
    {  -2.5703,   0.78969, -0.001202}, { -0.89264,   0.25121,   0.93573}, 
    {   1.6878,   0.25121,    1.1097}, {   3.5031,   0.25293,   0.93573}, 
    {  -2.5703,    1.0558, -0.001347}, { -0.89264,    1.0558,    1.0487}, 
    {   1.6878,    1.0558,    1.2437}, {   3.6342,    1.0527,    1.0487}, 
    {  -2.5703,    1.0558,         0}, { -0.89264,    1.0558,         0}, 
    {   1.6878,    1.0558,         0}, {   3.6342,    1.0527,         0}, 
    {  -2.5703,    1.0558,  0.001347}, { -0.89264,    1.0558,   -1.0487}, 
    {   1.6878,    1.0558,   -1.2437}, {   3.6342,    1.0527,   -1.0487}, 
    {  -2.5703,   0.78969,  0.001202}, { -0.89264,   0.25121,  -0.93573}, 
    {   1.6878,   0.25121,   -1.1097}, {   3.5031,   0.25293,  -0.93573}, 
    {   3.5031,   0.25293,         0}, {  -2.5703,   0.78969,         0}, 
    {   1.1091,    1.2179,         0}, {    1.145,     6.617,         0}, 
    {   4.0878,    1.2383,         0}, {  -2.5693,    1.1771, -0.081683}, 
    {  0.98353,    6.4948, -0.081683}, { -0.72112,    1.1364, -0.081683}, 
    {   0.9297,     6.454,         0}, {  -0.7929,     1.279,         0}, 
    {  0.91176,    1.2994,         0} 
}; 

const uint32 TriangleCount = 51;

uint32 Triangles[TriangleCount * 3] = { 
    4,   0,   5,   0,   1,   5,   1,   2,   5,   5,   2,   6,   3,   7,   2, 
    2,   7,   6,   5,   9,   4,   4,   9,   8,   5,   6,   9,   9,   6,  10, 
    7,  11,   6,   6,  11,  10,   9,  13,   8,   8,  13,  12,  10,  14,   9, 
    9,  14,  13,  10,  11,  14,  14,  11,  15,  17,  16,  13,  12,  13,  16, 
    13,  14,  17,  17,  14,  18,  15,  19,  14,  14,  19,  18,  16,  17,  20, 
    20,  17,  21,  18,  22,  17,  17,  22,  21,  18,  19,  22,  22,  19,  23, 
    20,  21,   0,  21,   1,   0,  22,   2,  21,  21,   2,   1,  22,  23,   2, 
    2,  23,   3,   3,  23,  24,   3,  24,   7,  24,  23,  15,  15,  23,  19, 
    24,  15,   7,   7,  15,  11,   0,  25,  20,   0,   4,  25,  20,  25,  16, 
    16,  25,  12,  25,   4,  12,  12,   4,   8,  26,  27,  28,  29,  30,  31, 
    32,  34,  33 
}; 

bool ComputePixelCoordinates(v3 WorldPoint, 
                             mat4 WorldToCamera, 
                             float CanvasBottom, // Represents Viewport dimensions
                             float CanvasLeft, 
                             float CanvasTop, 
                             float CanvasRight, 
                             float DistanceToNearPlane, 
                             uint32_t ImageWidth,
                             uint32_t ImageHeight,
                             i2 *RasterPoint) 
{ 
    v3 CameraSpacePoint = point_matrix_multiply(WorldPoint, WorldToCamera); 
    
    // Transforming Camera Space to Screen Space
    v2 ScreenSpacePoint; 
    ScreenSpacePoint.x = CameraSpacePoint.x / -CameraSpacePoint.z * DistanceToNearPlane; 
    ScreenSpacePoint.y = CameraSpacePoint.y / -CameraSpacePoint.z * DistanceToNearPlane; 
    
    // Transforming Screen Space to Normalized Device Coordinate Space
    v2 NDCPoint; 
    NDCPoint.x = (ScreenSpacePoint.x + CanvasRight) / (2 * CanvasRight); 
    NDCPoint.y = (ScreenSpacePoint.y + CanvasTop) / (2 * CanvasTop);
    
    // Transforming NDC Space to Raster Space
    RasterPoint->X = (int)(NDCPoint.x * ImageWidth); 
    RasterPoint->Y = (int)((1 - NDCPoint.y) * ImageHeight); 
    
    bool Visible = true; 
    if (ScreenSpacePoint.x < CanvasLeft || ScreenSpacePoint.x > CanvasRight || 
        ScreenSpacePoint.y < CanvasBottom || ScreenSpacePoint.y > CanvasTop) 
        Visible = false; 
    
    return Visible; 
} 

int main()
{
    // Camera Settings:
    float FocalLength = 35; // in mm, reducing the value increases the angle of view
    // 35mm Full Aperture in inches
    float FilmApertureWidth = 0.825; 
    float FilmApertureHeight = 0.446; 
    float InchToMm = 25.4; 
    float NearClippingPlanePos = 0.1; // Canvas is positioned at the near clipping plane
    float FarClipingPlanePos = 1000; 
    // image resolution in pixels
    uint32 ImageWidth = 512; 
    uint32 ImageHeight = 512;
    
    // Strategy for fitting the Film into the Device given the aspect ratio of both
    enum resolution_gate_fit { Fill = 0, Overscan };
    resolution_gate_fit FilmFit = Overscan;
    
    // Compute aspect ratio of camera and final image
    float FilmAspectRatio = FilmApertureWidth / FilmApertureHeight;
    float DeviceAspectRatio = ImageWidth / (float)ImageHeight;
    
    // Compute Canvas Coordinates
    float CanvasTop = ((FilmApertureHeight * InchToMm / 2) / FocalLength) * NearClippingPlanePos;
    float CanvasRight = ((FilmApertureWidth * InchToMm / 2) / FocalLength) * NearClippingPlanePos;
    float CanvasBottom;
    float CanvasLeft;
    
    float ScaleX = 1;
    float ScaleY = 1;
    
    switch(FilmFit)
    {
        default:
        case Fill:
        {
            if(FilmAspectRatio > DeviceAspectRatio)
            {
                ScaleX = DeviceAspectRatio / FilmAspectRatio;
            }
            else
            {
                ScaleY = FilmAspectRatio / DeviceAspectRatio;
            }
        } break;
        case Overscan:
        {
            if(FilmAspectRatio > DeviceAspectRatio)
            {
                ScaleY = FilmAspectRatio / DeviceAspectRatio;
            }
            else
            {
                ScaleX = DeviceAspectRatio / FilmAspectRatio;
            }
        } break;
    }
    
    CanvasTop *= ScaleX;
    CanvasRight *= ScaleY;
    CanvasBottom = -CanvasTop;
    CanvasLeft = -CanvasRight;
    
    printf("Screen window coordinates: %f %f %f %f\n", CanvasBottom, CanvasLeft, CanvasTop, CanvasRight); 
    printf("Film Aspect Ratio: %f\nDevice Aspect Ratio: %f\n", FilmAspectRatio, DeviceAspectRatio); 
    printf("Angle of view: %f (deg)\n", 2 * atan((FilmApertureWidth * InchToMm / 2) / FocalLength) * 180 / PI); 
    
    std::ofstream OutFile; 
    OutFile.open("./pinhole.svg"); 
    OutFile << "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" width=\"" << ImageWidth << "\" height=\"" << ImageHeight << "\">" << std::endl; 
    
    mat4 CameraToWorld = new_matrix(-0.95424, 0, 0.299041, 0, 
                                    0.0861242, 0.95763, 0.274823, 0, 
                                    -0.28637, 0.288002, -0.913809, 0, 
                                    -3.734612, 7.610426, -14.152769, 1); 
    
    mat4 WorldToCamera = invert_matrix(CameraToWorld); 
    
    // Canvas (Viewport) Dimensions
    float canvasWidth = 2, canvasHeight = 2; 
    
    for (uint32 i = 0; i < TriangleCount; ++i) { 
        v3 WorldPoint0 = Vertices[Triangles[i * 3]]; 
        v3 WorldPoint1 = Vertices[Triangles[i * 3 + 1]]; 
        v3 WorldPoint2 = Vertices[Triangles[i * 3 + 2]]; 
        i2 RasterPoint0, RasterPoint1, RasterPoint2; 
        
        bool Visible = true; 
        Visible &= ComputePixelCoordinates(WorldPoint0, WorldToCamera, 
                                           CanvasBottom, CanvasLeft, CanvasTop, CanvasRight, 
                                           NearClippingPlanePos, ImageWidth, ImageHeight, &RasterPoint0); 
        Visible &= ComputePixelCoordinates(WorldPoint1, WorldToCamera, 
                                           CanvasBottom, CanvasLeft, CanvasTop, CanvasRight, 
                                           NearClippingPlanePos, ImageWidth, ImageHeight, &RasterPoint1); 
        Visible &= ComputePixelCoordinates(WorldPoint2, WorldToCamera, 
                                           CanvasBottom, CanvasLeft, CanvasTop, CanvasRight, 
                                           NearClippingPlanePos, ImageWidth, ImageHeight, &RasterPoint2); 
        
        // Triangles outside of the Canvas region will be colored red
        int Color = Visible ? 0 : 255;
        OutFile << "<line x1=\"" << RasterPoint0.X << "\" y1=\"" << RasterPoint0.Y << "\" x2=\"" << RasterPoint1.X << "\" y2=\"" << RasterPoint1.Y << "\" style=\"stroke:rgb(" << Color << ",0,0);stroke-width:1\" />\n"; 
        OutFile << "<line x1=\"" << RasterPoint1.X << "\" y1=\"" << RasterPoint1.Y << "\" x2=\"" << RasterPoint2.X << "\" y2=\"" << RasterPoint2.Y << "\" style=\"stroke:rgb(" << Color << ",0,0);stroke-width:1\" />\n"; 
        OutFile << "<line x1=\"" << RasterPoint2.X << "\" y1=\"" << RasterPoint2.Y << "\" x2=\"" << RasterPoint0.X << "\" y2=\"" << RasterPoint0.Y << "\" style=\"stroke:rgb(" << Color << ",0,0);stroke-width:1\" />\n";
    }
    
    OutFile << "</svg>\n"; 
    OutFile.close(); 
    
    return 0; 
    
}