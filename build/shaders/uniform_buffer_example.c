#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matricies // Uniform block
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

// N == four byte entity
//
// Type	                     Layout rule
//
// Scalar e.g. int or bool	  Each scalar has a base alignment of N.
//
// Vector	                   Either 2N or 4N. This means that a vec3 has a base alignment of 4N.
//
// Array of scalars or vectors  Each element has a base alignment equal to that of a vec4.
//
// Matrices	                 Stored as a large array of column vectors, where each of those 
//                              vectors has a base alignment of vec4.
//
// Struct	                   Equal to the computed size of its elements according to the
//                              previous rules, but padded to a multiple of the size of a vec4.

layout (std140) uniform ExampleBlock
{
    //                  base allignment      aligned offset
    float value; //     4                    0
    vec3 vector; //     16                   16  (offset must be multiple of 16 so 4->16)
    mat4 matrix; //     16                   32  (column 0)
    //                  16                   48  (column 1)
    //                  16                   64
    //                  16                   80
    float values[3]; // 16                   96  (values[0])
    //                  16                   112 (values[1])
    //                  16                   128 (values[2])
    bool boolean; //    4                    144
    int integer;  //    4                    148
}

