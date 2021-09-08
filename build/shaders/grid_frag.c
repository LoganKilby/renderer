#version 330 core

in vec3 nearPoint;
in vec3 farPoint;
out vec4 outColor;

uniform mat4 projection;
uniform mat4 view;

vec4 Grid(vec3 fragPos3D, float scale) 
{
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    
    // blue z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color.z = 1.0;
    
    // red x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        color.x = 1.0;
    return color;
}

float ComputeDepth(vec3 pos) {
	// get the clip-space coordinates
	vec4 clip_space_pos = projection * view * vec4(pos.xyz, 1.0);
    
	// get the depth value in normalized device coordinates
	float clip_space_depth = clip_space_pos.z / clip_space_pos.w;
    
	// and compute the range based on gl_DepthRange
	float far = gl_DepthRange.far;
	float near = gl_DepthRange.near;
    
	float depth = (((far-near) * clip_space_depth) + near + far) / 2.0;
    
	return depth;
}

void main()
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    
    gl_FragDepth = ComputeDepth(fragPos3D);
    
    float spotlight;
    float fade = min(1.0, 1.5 - 0.02 * length(fragPos3D.xz));
    
    outColor = (Grid(fragPos3D, 10)) * float(t > 0);
    outColor.a *= fade;
}