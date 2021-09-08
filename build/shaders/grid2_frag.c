#version 330 core

out vec4 color;

in vec3 vP;
in vec3 near;
in vec3 far;

uniform mat4 projection;
uniform mat4 view;

float computeDepth(vec3 pos) {
	// get the clip-space coordinates
	vec4 clip_space_pos = projection * view * vec4(pos.xyz, 1.0);
    
	// get the depth value in normalized device coordinates
	float clip_space_depth = clip_space_pos.z / clip_space_pos.w;
    
	// and compute the range based on gl_DepthRange settings (not necessary with default settings, but left for completeness)
	float far = gl_DepthRange.far;
	float near = gl_DepthRange.near;
    
	float depth = (((far-near) * clip_space_depth) + near + far) / 2.0;
    
	return depth;
}

void main()
{
    float t = -near.y / (far.y - near.y);
    vec3 r = near + t * (far - near);
    float c = (int(round(r.x * 5.0)) + int(round(r.z * 5.0))) % 2;
    c = c * float(t > 0);
    
    float spotlight = min(1.0, 1.5 - 0.02 * length(r.xz));
    color = vec4(vec3(c * spotlight), 1);
    gl_FragDepth = computeDepth(r);
}