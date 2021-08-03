#include "renderer.cpp"

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

internal void
Render(win32_offscreen_buffer Buffer, game_input Input)
{
    // Most of the steps are simulated and only the rasterization using the edge function are
    // showcased. Canvas Bounding Box not implemented
    /*
    v2 Vertex0 = { 491.407, 411.407 };
    v2 Vertex1 = { 148.593, 68.5928 }; 
    v2 Vertex2 = { 148.593, 411.407 };
    */
    
    // Camera Space
    v3 Vertex2 = { -48, -10,  82}; 
    v3 Vertex1 = {  29, -15,  44}; 
    v3 Vertex0 = {  13,  34, 114}; 
    
    v3 Red   = { 1, 0, 0 };
    v3 Green = { 0, 1, 0 };
    v3 Blue  = { 0, 0, 1 };
    
    v2 ST2 = { 0, 0 }; // st coordiantes are usually what are called texture coordinates
    v2 ST1 = { 1, 0 }; 
    v2 ST0 = { 0, 1 };
    
    // Project triangle to screen space
    Vertex0.x /= Vertex0.z, Vertex0.y /= Vertex0.z;
    Vertex1.x /= Vertex1.z, Vertex1.y /= Vertex1.z;
    Vertex2.x /= Vertex2.z, Vertex2.y /= Vertex2.z;
    
    // Convert to NDC then to Raster space
    Vertex0.x = (1 + Vertex0.x) * 0.5 * Buffer.Width;
    Vertex0.y = (1 + Vertex0.y) * 0.5 * Buffer.Height;
    
    Vertex1.x = (1 + Vertex1.x) * 0.5 * Buffer.Width;
    Vertex1.y = (1 + Vertex1.y) * 0.5 * Buffer.Height;
    
    Vertex2.x = (1 + Vertex2.x) * 0.5 * Buffer.Width;
    Vertex2.y = (1 + Vertex2.y) * 0.5 * Buffer.Height;
    
    
    if(Input.Up) // If perspective correct vertex attribute interpolation is enabled
    {
        // Divide vertex-attributes by the vertex z coordinate
        /*
        Red.x /= Vertex0.z, Red.y /= Vertex0.z, Red.z /= Vertex0.z;
        Blue.x /= Vertex1.z, Blue.y /= Vertex1.z, Blue.z /= Vertex1.z;
        Green.x /= Vertex2.z, Green.y /= Vertex2.z, Green.z /= Vertex2.z;
        */
        
        ST0.x /= Vertex0.z, ST0.y /= Vertex0.z;
        ST1.x /= Vertex1.z, ST1.y /= Vertex1.z;
        ST2.x /= Vertex2.z, ST2.y /= Vertex2.z;
        
        // Pre-compute 1 / z
        Vertex0.z = 1 / Vertex0.z;
        Vertex1.z = 1 / Vertex1.z;
        Vertex2.z = 1 / Vertex2.z;
    }
    
    float TriangleArea = EdgeFunction(Vertex0, Vertex1, Vertex2);
    
    for(uint32 j = 0; j < Buffer.Height; ++j)
    {
        for(uint32 i = 0; i < Buffer.Width; ++i)
        {
            v3 Pixel = { i + 0.5f, Buffer.Height - j + 0.5f, 0 };
            float B0 = EdgeFunction(Vertex1, Vertex2, Pixel);
            float B1 = EdgeFunction(Vertex2, Vertex0, Pixel);
            float B2 = EdgeFunction(Vertex0, Vertex1, Pixel);
            
            if(B0 >= 0 && B1 >= 0 && B2 >= 0)
            {
                B0 /= TriangleArea;
                B1 /= TriangleArea;
                B2 /= TriangleArea;
                
                /*
                float R = (B0 * Red.x + B1 * Blue.x + B2 * Green.x) * 255;
                float G = (B0 * Red.y + B1 * Blue.y + B2 * Green.y) * 255;
                float B = (B0 * Red.z + B1 * Blue.z + B2 * Green.z) * 255;
*/
                
                float S = B0 * ST0.x + B1 * ST1.x + B2 * ST2.x;
                float T = B0 * ST0.y + B1 * ST1.y + B2 * ST2.y;
                
                if(Input.Up) // If perspective correct vertex attribute interpolation is enabled
                {
                    // If we use perspective correct interpolation, we need to multiply the result of this
                    // interpolation by z, the depth of the point on the 3D triangle that the
                    // pixel overlaps
                    
                    float Z = 1 / ((B0 * Vertex0.z) + (B1 * Vertex1.z) + (B2 * Vertex2.z));
                    //R *= Z, G *= Z, B *= Z;
                    S *= Z, T *= Z;
                }
                
                //SetPixel(Buffer, j, i, (unsigned char)R, (unsigned char)G, (unsigned char)B);
                int M = 10; // ?
                float P = (fmod(S * M, 1.0) > 0.5) ^ (fmod(T * M, 1.0) < 0.5);
                SetPixel(Buffer, 
                         j, 
                         i, 
                         (unsigned char)P * 255,
                         (unsigned char)P * 255,
                         (unsigned char)P * 255);
                
            }
            else
            {
                SetPixel(Buffer, j, i, 50, 50, 50);
            }
        }
    }
}