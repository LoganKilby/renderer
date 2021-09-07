#version 330 core

in vec3 nearPoint;
in vec3 farPoint;
in mat4 fragView;
in mat4 fragProj;

out vec4 outColor;

uniform float near;
uniform float far;

vec4 Grid(vec3 fragPos3D, float scale, bool drawAxis) 
{
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color.z = 1.0;
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        color.x = 1.0;
    return color;
}

float ComputeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

float ComputeLinearDepth(vec3 pos)
{
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    
    // put back between -1 and 1
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0;
    
    // get linear value between 0.01 and 100 (near and far plane)
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near));
    
    return linearDepth / far; // normalize
}

void main()
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    
    gl_FragDepth = ComputeDepth(fragPos3D);
    
    float linearDepth = ComputeLinearDepth(fragPos3D);
    float fade = max(0, (0.5 - linearDepth));
    
    outColor = (Grid(fragPos3D, 10, true) + Grid(fragPos3D, 1, true)) * float(t > 0);
    
    outColor.a *= fade;
}