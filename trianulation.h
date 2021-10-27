/* date = October 27th 2021 3:14 pm */

#ifndef TRIANULATION_H
#define TRIANULATION_H

/*

Delaunay 2D

1. Super-triangle: Begin by constructing a bounding triangle (volume in 3d?) large enough to contain the
 point set

2. Insert each point P into the triangulation. Find a triangle containing P by linearly walking over the
current triangles. (meshing a convex polygon sec. 13.4.2)

3. Two possible cases (figure 13.45):
3a. If P is interior to T and the circumcircle of T does not contain the opposite vertex of an adjacent
 triangle A, and A's circumcircle doesn't contain the opposite vertex of T, then T is split into three
 sub-triangle.
3b. If P is on the edge of a triangle, then each triangle sharing that edge are split into two sub-triangles

-SuperTriangle() computes the bounding triangle
-mesh object is a triangle mesh stored as vertex-edge-triangle table
-mesh.insert() function takes as input a triangle, either as three vertices or a triangle struct and inserts them into the mesh.
-mesh.remove() removes a triangle from the mesh
-triangle.v(i) accesses the index of the mesh vertex that the triangle shares
-The indices are ordered counter-clockwise
-The indexing is % 3, so triangle.v(0) and triangle.v(3) are the same integer
-triangle.adj(i) returns a reference to the adjacent triangle that shares the edge:
 <t.v(i), t.v(i + 1)>, and is null when there's no adjacent triangle to that edge
 -The object E is an edge
-E.adj(i) returns a reference to an adjacent triangle (at most two triangles)

-An infinite loop may occur when a triangle pair has all vertices on a common circumcircle. No four points
 in the input set can be cocircular. Evaluate both circumcircle tests, one for the current triangle
 configuration and one for the swapped configuration. If both tests indicate the swap should occur, then the
 swap should not be made.

-If the circumcircle tests accepts two adjacent triangles as input, pass the triangles in the same order.
 The two vertices that are opposite the shared edge have indices managed in the mesh obj. Pass the trianlges
 so that the first triangle has the vertex of the smallest index between the two triangles.

4. Any triangles that exist only because they share a vertex with the supertriangle should be removed.

*/

/*

Delaunay 3D

1. Construct a supersimplex that contains a hypersphere that itself contains the axis-aligned bounding box
 of the input points.

2. The supersimplex is added as the first simplex in a mesh of simplices. The mesh maintains the vertices
 and all the necessary connectivity between simplices. Moreover, the circumhypersphere for each simplex is
 stored to avoid having to recalculate centers and radii during the incremental updates.

3. Insert other data points one at a time and process:

3a. Determine which circumhypersphere contains the given point. Before the insertion, the empty condition is
 satisfied for all simplices. When the point is inserted, the simplices corresponding to the
 circumhyperspheres that violate the empty condition must be modified. The search for the containing
 hyperspheres is implemented as a search over simplices using the linear walk described in 2D, but the worst
 case runtime can be O(n^(d/2)). Once the containing simplex (or simplices if the point is on a shared
 boundary) is found, a depth-first search can be performed to find other simplices whose circumhyperspheres
 contain the input point. A linear search would also work, though potentially slower for large input sets.

3b. The union of the simplices who circumhyperspheres contain the input point form a d-dimensional
polyhedron called the insertion polyhedron. Locate the boundary faces of that polyhedron.

3c. Create new simplices by connecting the input point to the boundary faces. Then remove the old simplices
whose union was the insertion polyhedron.

4. After all points are inserted, those simplices that share a vertex of the sursimplex are removed from
the mesh. The resulting mesh is the delaunay triangulation of the points. 
*/


#endif //TRIANULATION_H
