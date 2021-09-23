#include "float.h"
#include "collision.h"
#include "math_util.h"
#include "renderer.h"
#include "entity.h"

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

inline rect
CreateRect(float X, float Y, float Width, float Height)
{
    rect Result;
    Result.X = X;
    Result.Y = Y;
    Result.Width = Width;
    Result.Height = Height;
    return Result;
}

internal rect
CreateRectFromDiagonalPoints(glm::vec2 A, glm::vec2 B)
{
    // Find bottom left corner and width,height
    float X, Y, Width, Height;
    if(A.x < B.x)
    {
        X = A.x;
        Width = B.x;
    }
    else
    {
        X = B.x;
        Width = A.x;
    }
    
    if(A.y < B.y)
    {
        Y = A.y;
        Height = B.y;
    }
    else
    {
        Y = B.y;
        Height = A.y;
    }
    
    return CreateRect(X, Y, Width, Height);
}

internal plane
CreatePlane(glm::vec3 Position, euler_angles Rotation)
{
    glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::mat4 RotationMatrix = mat4_EncodeEulerAnglesZYX(Rotation);
    
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
RayPlaneIntersection(ray Ray, plane Plane, glm::vec3 *IntersectionResult)
{
    float d = glm::length(Plane.Position);
    
    // Check if parallel
    float Denominator = glm::dot(Ray.Direction, Plane.Normal);
    if(abs(Denominator) < FLT_EPSILON)
    {
        // Check if the ray lies in the plane (by checking if the origin of the line
        // is in the plane).
        if(abs(glm::dot(Ray.Origin, Plane.Position)) < FLT_EPSILON)
        {
            *IntersectionResult = glm::vec3(0.0f);
            return true;
        }
        else
            return false;
    }
    
    float t = -(glm::dot(Plane.Normal, Ray.Origin) + d);
    Assert(Denominator); // It can be zero, right?
    t /= Denominator;
    
    if(t >= 0)
    {
        glm::vec3 Result = Ray.Origin + (t * Ray.Direction);
        *IntersectionResult = Result;
        return true;
    }
    
    return false;
}

internal bool
LinePlaneIntersection(line_segment Line, plane Plane)
{
    // Check if parallel
    float d = glm::length(Plane.Position);
    float Denominator = glm::dot(Line.Direction, Plane.Normal);
    if(abs(Denominator) < FLT_EPSILON)
    {
        // Check if the line lies in the plane (by checking if the origin of the line
        // is in the plane).
        if(abs(Line.P0.x * Plane.Position.x + Line.P0.y * Plane.Position.y + Line.P0.z * Plane.Position.z + d) < FLT_EPSILON)
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
    float Denominator = glm::dot(Ray.Direction, Plane.Normal);
    
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

internal bool
PointInRect(glm::vec2 Point, rect Rect)
{
    bool XAxisCollision = (Point.x >= Rect.X && Point.x <= Rect.Width) ? true : false;
    bool YAxisCollision = (Point.y >= Rect.Y && Point.y <= Rect.Height) ? true : false;
    return XAxisCollision && YAxisCollision;
}

internal glm::vec3
ScreenToWorldRay(glm::vec2 ScreenCoords, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{
    float X, Y, Width, Height;
    GetViewport(&X, &Y, &Width, &Height);
    
    // NOTE: I use (0, 0) as bottom left of screen
    float x = (2.0f * ScreenCoords.x) / Width - 1.0f;
    float y = (2.0f * ScreenCoords.y) / Height - 1.0f;
    
    // glm::vec2 NDC = glm::vec2(x, y);
    // glm::vec4 Clip = glm::vec4(RayNDC, -1.0f, 1.0f);
    glm::vec4 Eye = inverse(ProjectionMatrix) * glm::vec4(x, y, -1.0f, 1.0f);
    //RayEye = glm::vec4(Eye.x, Eye.y, -1.0f, 0.0f);
    glm::vec3 World = glm::inverse(ViewMatrix) * glm::vec4(glm::vec2(Eye), -1.0f, 0.0f);
    glm::vec3 Result = glm::normalize(World);
    
    return Result;
}

internal bool
PointInBoundedPlane(glm::vec3 BottomLeft, glm::vec3 TopRight, glm::vec3 Point)
{
    bool Result = Point.x >= BottomLeft.x && Point.x <= TopRight.x;
    Result &= Point.y >= BottomLeft.y && Point.y <= TopRight.y;
    
    // I think I need to find whether the plane lies on xy, xz, or yz
    // and test the rect in that plane specifically?
    
    return Result;
}