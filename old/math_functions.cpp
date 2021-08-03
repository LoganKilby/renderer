#include <cmath>
#include <algorithm>

#define PI 3.14159265358979323846

// TODO: PascalCase

// NOTE: Spherical vector coordinates:
// Theta represents the vertical angle and Phi represents the horizontal angle (in radians).
// Speherical coordinates encode cartesian coordinates in two numbers

// NOTE: Row-major order
// NOTE: (OpenGL) +Y is up, -Z is away
// NOTE: (OpenGL) FOV corresponds to vertical angle
// NOTE: (Maya) FOV corresponds to horizontal angle

enum axis
{
    X_AXIS = 0,
    Y_AXIS,
    Z_AXIS
};

struct i2
{
    int X;
    int Y;
};

struct angle2
{
    float theta;
    float phi;
};

struct v2
{
    float x;
    float y;
};

// TODO: Store these as indices int[3] = { x, y, z };
// NOTE: The distinction between vector and point and the usage of the math functions is very important.
// there is currently no point or vector data type, just v3, v4, v2 which represent a group of floats
struct v3
{
    float x;
    float y;
    float z;
};

struct v4
{
    float x;
    float y;
    float z;
    float w;
};

bool is_normalized_v3(v3);
v3 v3_subtract(v3, v3);
v3 normalize_vector(v3);
v3 cross_product(v3, v3);
// Two matirices m x p and p x n will result in a matrix of size m x n
// NOTE: Matrix row-axis correspondence
// [ a, b, c ] -> x-axis
// [ d, e, f ] -> y-axis
// [ g, h, i ] -> z-axis

// NOTE: Point(v3)-matrix multiplication for translation transorms
// [ a, b, c] -> x-axis
// [ d, e, f] -> y-axis
// [ g, h, i] -> z-axis
// [ M, M, M] -> encoded translation information

// NOTE: When point a * matrix m = point b, b * m-inverse = a
// and m * m-inverse = identity matrix. Useful when points or vectors need to be transformed to another
// coordinate space.

// NOTE: The diagonal values of the matrix scale the output
// If one of the points is negative, the position of a resulting point will be flipped
// will be flipped along the axis corresponding to that negative scalars position in the matrix.
// Note that this means the indices in the matrix correspond to an axis

// NOTE: Matrix components
//     
// [( c00 c01 c02 ) c03 ] -> (x axis) TranslateX
// [( c10 c11 c12 ) c13 ] -> (y axis) TranslateY
// [( c20 c21 c22 ) c23 ] -> (z axis) TranslateZ
// [  c30 c31 c32   c33 ] -> (encoded rotation, axis coordinates)
//                        -> c00 c11 c22 -> scale

// Order of operations: Scale * Rotation * Translate


// NOTE: Row major order
struct mat4
{
    float indices[4][4] = {};
};

mat4 new_matrix(float c00, float c01, float c02, float c03,
                float c10, float c11, float c12, float c13,
                float c20, float c21, float c22, float c23,
                float c30, float c31, float c32, float c33)
{
    mat4 result;
    
    result.indices[0][0] = c00;  
    result.indices[0][1] = c01;
    result.indices[0][2] = c02;  
    result.indices[0][3] = c03;
    
    result.indices[1][0] = c10;
    result.indices[1][1] = c11;
    result.indices[1][2] = c12;
    result.indices[1][3] = c13;
    
    result.indices[2][0] = c20;
    result.indices[2][1] = c21;
    result.indices[2][2] = c22;
    result.indices[2][3] = c23;
    
    result.indices[3][0] = c30;
    result.indices[3][1] = c31;
    result.indices[3][2] = c32;
    result.indices[3][3] = c33;
    
    return result;
}

angle2 cartesian_to_spherical(v3 vector)
{
    Assert(is_normalized_v3(vector)); // What if it's not normalized? then normalize it?
    
    float theta = acos(vector.z);
    float phi = atan2(vector.y, vector.x); // May need to be remapped to the range [0, 2PI]
    
    angle2 result = {};
    result.theta = theta;
    result.phi = phi;
    
    return result;
}

// NOTE: Spherical coordinates use a left-handed coordinate system in which the z-axis is the up vector
v3 spherical_to_cartersian(float theta, float phi)
{
    // TODO: Will the double precision make a big difference?
    return v3 { float(cos(phi) * sin(theta)), float(sin(phi) * sin(theta)), float(cos(theta)) };
}

// NOTE: arctangent: The function atan(px, py) doesn't take into account the sign of the parameters
// px & py. Use atan2(px, py) instead. Produces a result in the range [-PI, PI].
float radians(float degrees)
{
    return (PI / 180) * degrees;
}

float degrees(float radians)
{
    return radians * (180 / PI);
}

// NOTE: All theta functions require vectors of unit length
inline float theta(v3 vector)
{
    Assert(is_normalized_v3(vector));
    return acos(std::clamp<float>(vector.z, -1, 1));
}

inline float cos_theta(v3 vector)
{
    Assert(is_normalized_v3(vector));
    return vector.z;
}

inline float cos_theta_squared(v3 vector)
{
    Assert(is_normalized_v3(vector));
    return vector.z * vector.z;
}

inline float sin_theta_squared(v3 vector) 
{
    float result = 1 - cos_theta_squared(vector);
    
    return result > 0 ? result : 0;
}

inline float sin_theta(v3 vector)
{
    return sqrt(sin_theta_squared(vector));
}

inline float phi(v3 vector)
{
    // NOTE: atan2 will return a range from:
    // [0,  PI] if the vector is in the right part of the unit circle or
    // [0, -PI] if the vector is in the left part of the unit circle
    
    float phi = atan2(vector.y, vector.x);
    return (phi < 0) ? phi + (2 * PI) : phi;
}

inline float cos_phi(v3 vector)
{
    float s_theta = sin_theta(vector);
    if(s_theta == 0)
    {
        return 1;
    }
    
    return std::clamp<float>(vector.x / s_theta, -1, 1);
}

inline float sin_phi(v3 vector)
{
    float s_theta = sin_theta(vector);
    if(s_theta == 0)
    {
        return 0;
    }
    
    return std::clamp<float>(vector.y / s_theta, -1, 1);
}

mat4 identity_matrix()
{
    mat4 result;
    
    result.indices[0][0] = 1;
    result.indices[0][1] = 0;
    result.indices[0][2] = 0;
    result.indices[0][3] = 0;
    
    result.indices[1][0] = 0;
    result.indices[1][1] = 1;
    result.indices[1][2] = 0;
    result.indices[1][3] = 0;
    
    result.indices[2][0] = 0;
    result.indices[2][1] = 0;
    result.indices[2][2] = 1;
    result.indices[2][3] = 0;
    
    result.indices[3][0] = 0;
    result.indices[3][1] = 0;
    result.indices[3][2] = 0;
    result.indices[3][3] = 1;
    
    return result;
}

inline mat4 
copy_matrix(mat4 matrix)
{
    mat4 result;
    
    result.indices[0][0] = matrix.indices[0][0];
    result.indices[0][1] = matrix.indices[0][1];
    result.indices[0][2] = matrix.indices[0][2];
    result.indices[0][3] = matrix.indices[0][3];
    
    result.indices[1][0] = matrix.indices[1][0];
    result.indices[1][1] = matrix.indices[1][1];
    result.indices[1][2] = matrix.indices[1][2];
    result.indices[1][3] = matrix.indices[1][3];
    
    result.indices[2][0] = matrix.indices[2][0];
    result.indices[2][1] = matrix.indices[2][1];
    result.indices[2][2] = matrix.indices[2][2];
    result.indices[2][3] = matrix.indices[2][3];
    
    result.indices[3][0] = matrix.indices[3][0];
    result.indices[3][1] = matrix.indices[3][1];
    result.indices[3][2] = matrix.indices[3][2];
    result.indices[3][3] = matrix.indices[3][3];
    
    return result;
}

mat4 matrix_multiply(mat4 matrix1, mat4 matrix2)
{
    mat4 result = {};
    
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            result.indices[i][j] = 
                matrix1.indices[i][0] * matrix2.indices[0][j] +
                matrix1.indices[i][1] * matrix2.indices[1][j] +
                matrix1.indices[i][2] * matrix2.indices[2][j] +
                matrix1.indices[i][3] * matrix2.indices[3][j];
        }
    }
    
    return result;
}

inline mat4 
uniform_scale_matrix(float scalar)
{
    // c00 c11 c22 -> scale
    mat4 result = identity_matrix();
    
    result.indices[0][0] = scalar;
    result.indices[1][1] = scalar;
    result.indices[2][2] = scalar;
    
    return result;
}

inline mat4
translation_matrix(float x, float y, float z)
{
    mat4 result = identity_matrix();
    
    result.indices[0][3] = x;
    result.indices[1][3] = y;
    result.indices[2][3] = z;
    
    return result;
}

mat4 
rotation_matrix(float x_rotation, float y_rotation, float z_rotation)
{
    // NOTE: This function expects rotation in radians
    
    mat4 x_axis = identity_matrix();
    mat4 y_axis = identity_matrix();
    mat4 z_axis = identity_matrix();
    
    if(x_rotation)
    {
        x_axis.indices[1][1] = cos(x_rotation);
        x_axis.indices[1][2] = -sin(x_rotation);
        x_axis.indices[2][1] = sin(x_rotation);
        x_axis.indices[2][2] = cos(x_rotation);
    }
    
    if(y_rotation)
    {
        y_axis.indices[0][0] = cos(y_rotation);
        y_axis.indices[0][2] = sin(y_rotation);
        y_axis.indices[2][0] = -sin(y_rotation);
        y_axis.indices[2][2] = cos(y_rotation);
    }
    
    if(z_rotation)
    {
        z_axis.indices[0][0] = cos(z_rotation);
        z_axis.indices[0][1] = -sin(z_rotation);
        z_axis.indices[1][0] = sin(z_rotation);
        z_axis.indices[1][1] = cos(z_rotation);
    }
    
    mat4 result = matrix_multiply(z_axis, y_axis);
    result = matrix_multiply(result, x_axis);
    
    return result;
}

// NOTE: The order in which a point gets rotated around multiple rotation matrices matters.
// The result of p * x_axis * y_axis != p * y_axis * x_axis
// NOTE: rotating by more than 360 degrees can cause issues, namely "gimbal lock"
v3 axis_rotation(v3 point, float radians, enum axis axis)
{
    v3 result = {};
    
    switch(axis)
    {
        case X_AXIS:
        {
            double x_axis_matrix[3][3] = 
            {
                { 1, 0,           0          },
                { 0, cos(radians), -sin(radians) },
                { 0, sin(radians), cos(radians) }
            };
            
            result.x = 
                point.x * x_axis_matrix[0][0] +
                point.y * x_axis_matrix[1][0] +
                point.z * x_axis_matrix[2][0];
            result.y = 
                point.x * x_axis_matrix[0][1] +
                point.y * x_axis_matrix[1][1] +
                point.z * x_axis_matrix[2][1];
            result.z = 
                point.x * x_axis_matrix[0][2] +
                point.y * x_axis_matrix[1][2] +
                point.z * x_axis_matrix[2][2];
            
        } break;
        case Y_AXIS:
        {
            double y_axis_matrix[3][3] = 
            {
                { cos(radians), 0, sin(radians) },
                { 0,          1, 0           }, 
                { -sin(radians), 0, cos(radians)  }
            };
            
            result.x = 
                point.x * y_axis_matrix[0][0] +
                point.y * y_axis_matrix[1][0] +
                point.z * y_axis_matrix[2][0];
            result.y = 
                point.x * y_axis_matrix[0][1] +
                point.y * y_axis_matrix[1][1] +
                point.z * y_axis_matrix[2][1];
            result.z = 
                point.x * y_axis_matrix[0][2] +
                point.y * y_axis_matrix[1][2] +
                point.z * y_axis_matrix[2][2];
            
        } break;
        case Z_AXIS:
        {
            double z_axis_matrix[3][3] = 
            {
                { cos(radians), -sin(radians), 0 },
                { sin(radians), cos(radians), 0 },
                { 0,           0,          1 }
            };
            
            result.x = 
                point.x * z_axis_matrix[0][0] +
                point.y * z_axis_matrix[1][0] +
                point.z * z_axis_matrix[2][0];
            result.y = 
                point.x * z_axis_matrix[0][1] +
                point.y * z_axis_matrix[1][1] +
                point.z * z_axis_matrix[2][1];
            result.z = 
                point.x * z_axis_matrix[0][2] +
                point.y * z_axis_matrix[1][2] +
                point.z * z_axis_matrix[2][2];
            
        } break;
        default:
        {
            return point;
        }
    }
    
    return result;
}

// NOTE: Swaps the rows with the columns
mat4 transpose_matrix(mat4 matrix)
{
    mat4 result = {};
    
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            result.indices[i][j] = matrix.indices[j][i];
        }
    }
    
    return result;
}

mat4 invert_matrix(mat4 t) 
{ 
    int i, j, k; 
    mat4 s = identity_matrix(); 
    
    // Forward elimination
    for (i = 0; i < 3 ; i++) { 
        int pivot = i; 
        
        float pivotsize = t.indices[i][i]; 
        
        if (pivotsize < 0) 
            pivotsize = -pivotsize; 
        
        for (j = i + 1; j < 4; j++) { 
            float tmp = t.indices[j][i]; 
            
            if (tmp < 0) 
                tmp = -tmp; 
            
            if (tmp > pivotsize) { 
                pivot = j; 
                pivotsize = tmp; 
            } 
        } 
        
        if (pivotsize == 0) { 
            // Cannot invert singular matrix
            Assert(0);
            return identity_matrix(); 
        } 
        
        if (pivot != i) { 
            for (j = 0; j < 4; j++) { 
                float tmp; 
                
                tmp = t.indices[i][j]; 
                t.indices[i][j] = t.indices[pivot][j]; 
                t.indices[pivot][j] = tmp; 
                
                tmp = s.indices[i][j]; 
                s.indices[i][j] = s.indices[pivot][j]; 
                s.indices[pivot][j] = tmp; 
            } 
        } 
        
        for (j = i + 1; j < 4; j++) { 
            float f = t.indices[j][i] / t.indices[i][i]; 
            
            for (k = 0; k < 4; k++) { 
                t.indices[j][k] -= f * t.indices[i][k]; 
                s.indices[j][k] -= f * s.indices[i][k]; 
            } 
        } 
    } 
    
    // Backward substitution
    for (i = 3; i >= 0; --i) { 
        float f; 
        
        if ((f = t.indices[i][i]) == 0) { 
            // Cannot invert singuar matrix
            Assert(0);
            return identity_matrix(); 
        } 
        
        for (j = 0; j < 4; j++) { 
            t.indices[i][j] /= f; 
            s.indices[i][j] /= f; 
        } 
        
        for (j = 0; j < i; j++) { 
            f = t.indices[j][i]; 
            
            for (k = 0; k < 4; k++) { 
                t.indices[j][k] -= f * t.indices[i][k]; 
                s.indices[j][k] -= f * s.indices[i][k]; 
            } 
        } 
    } 
    
    return s; 
} 

v3 vector_matrix_multiply(v3 vector, mat4 matrix)
{
    v3 result = {};
    
    result.x = 
        vector.x * matrix.indices[0][0] + 
        vector.y * matrix.indices[0][1] + 
        vector.z * matrix.indices[0][2] +
        matrix.indices[0][3];
    result.y = 
        vector.x * matrix.indices[1][0] + 
        vector.y * matrix.indices[1][1] + 
        vector.z * matrix.indices[1][2] +
        matrix.indices[1][3];
    result.z = 
        vector.x * matrix.indices[2][0] + 
        vector.y * matrix.indices[2][1] + 
        vector.z * matrix.indices[2][2] +
        matrix.indices[2][3];
    
    return result;
}

inline mat4
translate_rotate_scale_matrix(mat4 translation, mat4 rotation, mat4 scale)
{
    mat4 result = matrix_multiply(translation, rotation);
    result = matrix_multiply(result, scale);
    return result;
}

inline mat4
projection_view_model_matrix(mat4 projection, mat4 view, mat4 model)
{
    mat4 result = matrix_multiply(projection, view);
    result = matrix_multiply(result, model);
    return result;
}

mat4 perspective_matrix(float field_of_view, float aspect_ratio, float near_plane, float far_plane)
{
    float half_angle = field_of_view * 0.5f;
    float scale = tan(radians(half_angle)) * near_plane;
    
    float right = aspect_ratio * scale;
    float left = -right;
    float top = scale;
    float bottom = -top;
    
    mat4 result = {};
    
    // NOTE: Row-major
    result.indices[0][0] = 2 * near_plane / (right - left);
    result.indices[1][1] = 2 * near_plane / (top - bottom);
    result.indices[2][0] = (right + left) / (right - left);
    result.indices[2][1] = (top + bottom) / (top - bottom);
    result.indices[2][2] = -((far_plane + near_plane) / (far_plane - near_plane));
    result.indices[2][3] = -1;
    result.indices[3][2] = -((2 * far_plane * near_plane) / (far_plane - near_plane));
    
    return result;
}

mat4 camera_lookat_matrix(v3 camera_from, v3 camera_to, v3 world_up_vector)
{
    
    v3 camera_forward = normalize_vector(v3_subtract(camera_from, camera_to));
    v3 camera_right = cross_product(normalize_vector({0, 1, 0}), camera_forward);
    v3 camera_up = cross_product(camera_forward, camera_right);
    
    //camera_translate_matrix = transpose_matrix(camera_translate_matrix);
    
    mat4 camera_space_matrix = {};
    camera_space_matrix.indices[0][0] = camera_right.x;
    camera_space_matrix.indices[0][1] = camera_right.y;
    camera_space_matrix.indices[0][2] = camera_right.z;
    camera_space_matrix.indices[1][0] = camera_up.x;
    camera_space_matrix.indices[1][1] = camera_up.y;
    camera_space_matrix.indices[1][2] = camera_up.z;
    camera_space_matrix.indices[2][0] = camera_forward.x;
    camera_space_matrix.indices[2][1] = camera_forward.y;
    camera_space_matrix.indices[2][2] = camera_forward.z;
    camera_space_matrix.indices[3][0] = -camera_from.x;
    camera_space_matrix.indices[3][1] = -camera_from.y;
    camera_space_matrix.indices[3][2] = -camera_from.z;
    camera_space_matrix.indices[3][3] = 1;
    // The camera space matrix is transposed because the movements of the objects in the space
    // are visually inverted. When the camera goes "up" the objects in the world should go "down", etc
    camera_space_matrix = transpose_matrix(camera_space_matrix);
    
    return camera_space_matrix;
}

glm::mat4 glm_lookat_matrix(glm::vec3 camera_from, glm::vec3 camera_to, glm::vec3 world_up_vector)
{
    glm::vec3 camera_forward = glm::normalize(camera_from - camera_to); // z-axis
    glm::vec3 camera_right = glm::cross(glm::normalize(world_up_vector), camera_forward); // x-axis
    glm::vec3 camera_up = glm::cross(camera_forward, camera_right); // y-axis
    
    glm::mat4 translation = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    
    translation[3][0] = -camera_from.x;
    translation[3][1] = -camera_from.y;
    translation[3][2] = -camera_from.z;
    
    rotation[0][0] = camera_right.x;
    rotation[1][0] = camera_right.y;
    rotation[2][0] = camera_right.z;
    rotation[0][1] = camera_up.x;
    rotation[1][1] = camera_up.y;
    rotation[2][1] = camera_up.z;
    rotation[0][2] = camera_forward.x;
    rotation[1][2] = camera_forward.y;
    rotation[2][2] = camera_forward.z;
    
    return rotation * translation;
}

v3 point_matrix_multiply(v3 point, mat4 matrix)
{
    v3 result = {};
    
    result.x = 
        point.x * matrix.indices[0][0] + 
        point.y * matrix.indices[1][0] + 
        point.z * matrix.indices[2][0] + 
        matrix.indices[3][0];
    result.y =
        point.x * matrix.indices[0][1] + 
        point.y * matrix.indices[1][1] + 
        point.z * matrix.indices[2][1] + 
        matrix.indices[3][1];
    result.z =
        point.x * matrix.indices[0][2] + 
        point.y * matrix.indices[1][2] + 
        point.z * matrix.indices[2][2] + 
        matrix.indices[3][2];
    float w = 
        point.x * matrix.indices[0][3] + 
        point.y * matrix.indices[1][3] + 
        point.z * matrix.indices[2][3] + 
        matrix.indices[3][3];
    
    if(w != 1 && w != 0)
    {
        result.x /= w;
        result.y /= w;
        result.z /= w;
    }
    
    return result;
}

v3 cross_product(v3 a, v3 b)
{
    // NOTE: On the cross product:
    
    // Returns a vector orthogonal to a and b
    
    // If a and b are parallel, the resulting vector is { 0, 0, 0 }
    
    // When a and b are normalized and orthogonal to each other, the output will be the
    // vector forming a cartesian coordinate system. a, b, (c)
    
    // The handedness of the resulting vector is based on the order of multiplication.
    // a * b = c, b * a = -c
    
    v3 result = {};
    
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    
    return result;
}

float dot_product(v3 a, v3 b)
{
    // NOTE: On the dot product:
    
    // If b is a unit vector then the product a * b gives ||A||cos(theta), the magnitude of the
    // projection of a in the direction of b. "Scalar projection"
    
    // If neither a nor b is a unit vector, we can write that a * b / ||b|| since b as a unit
    // vector is b / ||b||
    
    // If both vectors are normalized then taking the arc cosine of the dot product gives the angle
    // between the two vectors. 
    
    // When two vectors are perpendicular to one another, the dot product will equal 0
    
    // When two vectors are pointing in opposite directions, the dot product will equal -1
    
    // When two vectors are parallel, the dot product will equal 1
    
    float result = {};
    result += a.x * b.x;
    result += a.y * b.y;
    result += a.z * b.z;
    return result;
}

v3 v3_subtract(v3 a, v3 b)
{
    v3 result;
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    
    return result;
}

inline float angle_between_vectors(v3 a, v3 b)
{
    return degrees(acos(dot_product(a, b)));
}

float vector_length(v3 vector)
{
    
    // NOTE: The length of a vector can be found by taking the sqrt of
    // a vector's dot product of itself: sqrt(dot_product_v3(vector, vector));
    
    float result = sqrt(vector.x * vector.x +
                        vector.y * vector.y +
                        vector.z * vector.z);
    return result;
}

bool is_normalized_v3(v3 vector)
{
    if(vector.x <= 1 &&
       vector.y <= 1 &&
       vector.z <= 1)
    {
        return true;
    }
    
    return false;
}

v3 normalize_vector(v3 vector)
{
    float length = dot_product(vector, vector);
    if(length > 0)
    {
        float inverse_length = 1 / sqrt(length); // inverse length
        vector.x *= inverse_length;
        vector.y *= inverse_length;
        vector.z *= inverse_length;
    }
    
    return vector;
}

// NOTE: Local Coordinate Systems
// At point P of a surface, the normal N, tangent T, and bi-tangent B make up a plane tangent to P at the
// surface. Taking the cross product of B and T give N. The direction T and B are facing depends on
// whether N is facing inward or outward. The direction can be found using the right-handed rule
// Matrix representation:
// If the UP vector is the Y-axis
// [Tx, Ty, Tz, 0]
// [Nx, Ny, Nz, 0]
// [Bx, By, Bz, 0]
// [ 0,  0,  0, 1]

// If the UP vector is the Z-axis:
// [Tx, Ty, Tz, 0]
// [Bx, By, Bz, 0]
// [Nx, Ny, Nz, 0]
// [ 0,  0,  0, 1]

// Multiplying a vector by this matrix will define new vector coordinates in terms of the local
// coordinate system define by the matrix.

// Evidently it is common to use to the Z-asix as the UP vector during shading. This would require a
// transformation using the matrix above.

// NOTE: Surface Normals
// When we know the tangent T and bi-tangent B vectors at P, the surface normal can be computed
// by the dot product of T and B. T * B = N and B * T = -N

// To transform a normal N, multiply it by the inverse transpose of the matrix used to transform its 
// vector or point. 
// N' = N * M(-1 Transpose)
// Orthogonal Matrix O: O(T) = O(-1), O = O(-1 T)
// This gives a normal that is orthogonal to the point or vector. Not doing this can
// result in the normal being scaled incorrectly and no longer perpendicular.
