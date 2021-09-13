float PointLineDistanceSquared(glm::vec3 Point, glm::vec3 Line)
{
    
}

/*
Plane intersections with planar components and linear components
Line: L(t) = P + td, 0, <= t <= infinity
Ray: R(t) = P + td, 0 <= t <= infinity, usualy defined with a normalized vector
Line to Ray: S(t) = P0 + t(P1 - P0)
Plane (implicit form) ax + by + cz + D = 0, n = (a, b, c)
Plane (parametric form) X(s, t) = P + su + tv. 
Plane (normal point form) n dot (X - P) = 0
Plane (normal constant form) n dot X = c

implicit form(1): ax + by + cz + D = 0
a^2 + b^2 + c^2 = 1
n = [a, b, c] represents the normal to the plane
|D| represents the minimum distance from the plane to the origin [0, 0, 0]

The intersection of the linear component L and P (if it exists) is at point
(2) Q = P + td for some t. 
Since Q is a point on P, it must also satisfy the implicit plane equation.
We can substitute (2) into (1):

a(P.x + (D.x)t) + b(P.y + (D.y)t) + c(P.z + (D.z)t) + D = 0

and solve for parameter t:

t = (-aP.x + bP.y + cP.z + D) / (aD.x, + bD.y + cD.z)

or in terms of vectors:

t = -(dot(N, P) + D) / dot(N, D)

Note that the denominator dot(N, D) is the dot product of the normal and the ray's direction. If
this values is equal to zero, then the ray and plane are parallel. If the ray is in the plane,
there are infinitely many intersections, and if the ray is not in the plane, there are no
intersections.



*/