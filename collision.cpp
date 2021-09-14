#include "float.h"
#include "collision.h"
#include "math_util.h"

/*
Plane intersections with planar components and linear components
Line: L(t) = P + td, 0, <= t <= infinity
Ray: R(t) = P + td, 0 <= t <= infinity, usualy defined with a normalized vector
Line to Ray: S(t) = P0 + t(P1 - P0)
Plane (implicit form) ax + by + cz + D = 0, n = (a, b, c)
Plane (parametric form) X(s, t) = P + su + tv. 
Plane (normal point form) n dot (X - P) = 0
Plane (normal constant form) n dot X = c

implicit form: ax + by + cz + D = 0
a^2 + b^2 + c^2 = 1
n = [a, b, c] represents the normal to the plane
|D| represents the minimum distance from the plane to the origin [0, 0, 0]

The intersection of the linear component L and P (if it exists) is at point
Q = P + td, for some t (2)
Since Q is a point on P, it must also satisfy the implicit plane equation.
We can substitute (2) into (1):

a(P.x + (D.x)t) + b(P.y + (D.y)t) + c(P.z + (D.z)t) + D = 0

and solve for parameter t:

t = -(aP.x + bP.y + cP.z + d) / (aD.x, + bD.y + cD.z)
(2) where D is the normal vector and d is the distance from the plane to the origin

Then plug t into equation (2): Q = P + td, where d is the direction vector

or in terms of vectors:

t = -(dot(N, P + d)) / dot(N, D)
If the dot product of the plane normal and the ray direction == 0, the ray and plane are parallel.
If calculating a ray, t must be >= 0, or no collision

Note that the denominator dot(N, D) is the dot product of the normal and the ray's direction. If
this values is equal to zero, then the ray and plane are parallel. If the ray is in the plane,
there are infinitely many intersections, and if the ray is not in the plane, there are no
intersections.
*/

// NOTE: Debug quad normal: 0.0f, 0.0f, 1.0f

internal plane
CreatePlane(glm::vec3 Position, glm::vec3 Normal, euler_angles Rotation)
{
    // Origin is (0, 0, 0)
    // Find normal ... rotate normal
    // calculate distance from origin ... is just length(Position);
    
    glm::mat4 RotationMatrix = Mat4_EncodeEulerAnglesZYX(Rotation);
    
    plane Result = {};
    Result.Position = Position;
    Result.Normal = RotationMatrix * glm::vec4(Normal, 1.0f);
    
    return Result;
}

internal ray
RayFromPoints(glm::vec3 P0, glm::vec3 P1)
{
    ray Result;
    Result.Direction = P1 - P0;
    Result.Origin = P0;
}

internal bool
RayPlaneIntersection(ray Ray, plane Plane, float *Result)
{
    float d = glm::length(Plane.Position);
    
    // Check if parallel
    float Denominator = Dot(Ray.Direction, Plane.Normal);
    if(abs(Denominator) < FLT_EPSILON)
    {
        // Check if the line lies in the plane (by checking if the origin of the line
        // is in the plane).
        if(abs(Ray.Origin.x * Plane.Position.x + Ray.Origin.y * Plane.Position.y + Ray.Origin.z * Plane.Position.z + d) < FLT_EPSILON)
        {
            *Result = 0;
            return true;
        }
        else
            return false;
    }
    
    float t = -(Plane.Position.x * Ray.Origin.x + Plane.Position.y * Ray.Origin.y + Plane.Position.z * Ray.Origin.z + d);
    
    if(t >= 0)
    {
        *Result = t;
        return true;
    }
    
    return false;
}

internal bool
LinePlaneIntersection(line Line, plane Plane)
{
    // Check if parallel
    float d = glm::length(Plane.Position);
    float Denominator = Dot(Line.Direction, Plane.Normal);
    if(abs(Denominator) < FLT_EPSILON)
    {
        // Check if the line lies in the plane (by checking if the origin of the line
        // is in the plane).
        if(abs(Line.Origin.x * Plane.Position.x + Line.Origin.y * Plane.Position.y + Line.Origin.z * Plane.Position.z + d) < FLT_EPSILON)
        {
            return true;
        }
        else
            return false;
    }
    
    // Nonparallel, so compute intersection
    // t = -(Plane.a * Line.Origin.x + Plane.b * Line.Origin.y + Plane.c * Line.Origin.z + Plane.d);
    // t = t / Denominator;
    // Intersection = Line.Origin + t * Line.Direction;
    return true;
}


internal bool
LineSegmentPlaneIntersection(glm::vec3 P0, glm::vec3 P1, plane Plane)
{
    // Convert line segment to ray
    // R(t) = P0 + t(P1 - P0)
    // t is defined between [0, 1], we can reject values outisde of this range
    ray Ray = RayFromPoints(P0, P1);
    float d = glm::length(Plane.Position);
    float Denominator = Dot(Ray.Direction, Plane.Normal);
    
    if(abs(Denominator) < FLT_EPSILON)
    {
        // Check if the line lies in the plane (by checking if the origin of the line
        // is in the plane).
        if(abs(Ray.Origin.x * Plane.Position.x + Ray.Origin.y * Plane.Position.y + Ray.Origin.z * Plane.Position.z + d) < FLT_EPSILON)
        {
            return true;
        }
        else
            return false;
    }
    
    float t = -(Plane.Position.x * Ray.Origin.x + Plane.Position.y * Ray.Origin.y + Plane.Position.z * Ray.Origin.z + d);
    
    return (t >= 0 && t <= 1) ? true : false;
    
}