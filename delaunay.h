/* date = October 27th 2021 3:14 pm */

#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <pcl/pcl_base.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/io/vtk_io.h>
#include <pcl/surface/organized_fast_mesh.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/surface/mls.h>

#define start_counter() QPC_StartCounter()
#define end_counter() QPC_EndCounter()
#define end_counter_print(msg) QPC_EndCounterPrint(msg)

static void
triangulate_point_set(vertex_buffer *VertexBuffer)
{
    start_counter();
    
    // PointXYZ's are 4-byte alligned
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    
#if 1
    cloud->reserve(VertexBuffer->ElementCount);
    
    f32 XRange = 1.0f / (VertexBuffer->MaxX - VertexBuffer->MinX);
    f32 YRange = 1.0f / (VertexBuffer->MaxY - VertexBuffer->MinY);
    f32 ZRange = 1.0f / (VertexBuffer->MaxZ - VertexBuffer->MinZ);
    for(int i = 0; i < VertexBuffer->ElementCount; ++i)
    {
        cloud->push_back(pcl::PointXYZ((VertexBuffer->PointAttribute[i].x - VertexBuffer->MinX) * XRange, (VertexBuffer->PointAttribute[i].y - VertexBuffer->MinY) * YRange, (VertexBuffer->PointAttribute[i].z - VertexBuffer->MinZ) * ZRange));
        
        //cloud->push_back(pcl::PointXYZ(VertexBuffer->PointAttribute[i].x, VertexBuffer->PointAttribute[i].y, VertexBuffer->PointAttribute[i].z));
    }
#else
    pcl::PCLPointCloud2 cloud_blob;
    pcl::io::loadPCDFile ("bun0.pcd", cloud_blob);
    pcl::fromPCLPointCloud2 (cloud_blob, *cloud);
#endif
    
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
    // Normal estimation*
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
    pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud(cloud);
    n.setInputCloud(cloud);
    n.setSearchMethod(tree);
    n.setKSearch(20);
    n.compute(*normals);
    //* normals should not contain the point normals + surface curvatures
    
    // Concatenate the XYZ and normal fields*
    pcl::concatenateFields (*cloud, *normals, *cloud_with_normals);
    //* cloud_with_normals = cloud + normals
    
    // Create search tree*
    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud(cloud_with_normals);
    
    // Initialize objects
    pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
    pcl::PolygonMesh triangles;
    
    // Set the maximum distance between connected points (maximum edge length)
    gp3.setSearchRadius(FLT_MAX);
    
    // Set typical values for the parameters
    gp3.setMu(2);
    gp3.setMaximumNearestNeighbors(200);
    gp3.setMaximumSurfaceAngle(M_PI/2); // 45 degrees
    gp3.setNormalConsistency(false);
    
    gp3.setMinimumAngle(M_PI); // 10 degrees
    gp3.setMaximumAngle(4*M_PI); // 120 degrees
    
    // Get result
    gp3.setInputCloud(cloud_with_normals);
    gp3.setSearchMethod(tree2);
    gp3.reconstruct(triangles);
    
    // Additional vertex information
    std::vector<int> parts = gp3.getPartIDs();
    std::vector<int> states = gp3.getPointStates();
    
    pcl::io::saveVTKFile ("mesh.vtk", triangles);
    
    end_counter_print("triangulation: ");
}

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

#if 0

struct face
{
    b32 visited;
    u32 count;
    u32 *vertex_index_list;
};

struct triangle
{
    
};

struct edge
{
    u32 indices[2]; // vertex index
    
    // Cannot be empty since an edge in the mesh must belong to a face. The edge is classified by the
    // number of facing sharing it.
    
    // Boundary edge: one face
    // Manifold/Interior edge: two faces
    // Junction edge: greater than two faces
    u32 face_count;
    face *faces;
};

struct vertex_index_list
{
    u32 vertex_index;
    vertex_index_list *v_list;
};

struct vertex
{
    u32 index;
    
    // must not be empty
    u32 edge_count;
    edge *edges;
    
    // must not be empty
    u32 face_count;
    face *faces;
};

struct vef_table
{
    u32 vertex_count;
    vertex *vertices;
    
    u32 edge_count;
    edge *edges;
    
    u32 face_count;
    u32 *faces; // vertex array indices (ordered)
};

internal void
super_triangle(v3 *points, u32 point_count)
{
    
    f32 min_x = FLT_MAX, max_x = 0, min_y = FLT_MAX, max_y = 0, min_z = FLT_MAX, max_z = 0;
    
    v3 v;
    for(int i = 0; i < point_count; ++i)
    {
        v = points[i];
        
        if(v.x < min_x)
        {
            min_x = points[i].x;
        }
        else if(v.x > max_x)
        {
            max_x = v.x;
        }
        
        if(v.y < min_y)
        {
            min_y = v.y;
        }
        else if(v.y > max_y)
        {
            max_y = v.y;
        }
        
        
        if(v.z < min_z)
        {
            min_z = v.z;
        }
        else if(v.z > max_z)
        {
            max_z = v.z;
        }
    }
    
    // bounding box: min_x, max_x, min_y, max_y, min_z, max_z
    
    // eberly
    // fitting a sphere to 3d points (A.8; pg: 887)
    // sphere and axis-aligned bounding box (11.12.8; pg: 644)
}

struct triangle_mesh
{
    
};

internal void
triangulate_points(v3 *points, u32 point_count)
{
    vef_table table;
    
    triangle_mesh mesh = {};
    // TODO: Find the bounding box of the point set in prepass
    mesh_insert(&mesh, super_triangle(points, point_count));
    
    for(u32 i = 0; i < point_count; ++i)
    {
        c = mesh_extract(points[i]);
        if(c.type == TRIANGLE)
        {
            // points[i] splits t into three subtriangles
            triangle t = c;
            for(int j = 0; j < 3; ++j)
            {
                // insert subtriangles into mesh
                n = mesh.insert(i, t.vertices(j), t.vertices(j + 1));
                
                // n and adjacent trianlge might need edge swap
                a = t.adjacent(j);
                if(a)
                {
                    stack.push(n, a);
                }
            }
        }
        else // edge
        {
            edge e = c;
            // points[i] splits each triangle sharing e into two subtriangles
            for(int k = 0; k <= 1; ++k)
            {
                triangle t = e.adjacent(k);
                if(t)
                {
                    for(int j = 0; j < 3; ++j)
                    {
                        if(t.edge(i) != c)
                        {
                            // insert subtriangle into mesh
                            n = mesh.insert(i, t.vertices(j), t.vertices(j + 1));
                            
                            // n and adjacent triangle might need edge swap
                            a = t.adjacent(j);
                            if(a)
                            {
                                stack.push(n, a);
                            }
                        }
                    }
                }
                else
                {
                    mesh.remove(t);
                }
            }
        }
        
        // relevant trianlges containing points[i] have been subdivided. Now process pairs of triangles
        // that might need an edge swapped to preserve the empty circumcircle constraint
        while(!stack.empty())
        {
            stack.pop(t, a);
            
            // 13.45
            // compute i0, i1, i2, i3 with t.v(i1) = a.v(i2) and t.v(i2) = a.v(i1)
            f32 i0, i1, i2, i3;
            circumcircle a;
            if(in_circumcircle(a, t.v(i0)))
            {
                n0 = mesh.insert(t.v(i0), t.v(i1), a.v(i3));
                b0 = a.adjacent(i1);
                
                if(b0)
                {
                    stack.push(n0, b0); // now <n0, b0> might need swapping
                }
                
                n1 = mesh.insert(t.v(i0), a.v(i3), a.v(i2));
                b1 = a.adjacent(i3);
                if(b1)
                {
                    stack.push(n1, b1); // now <n1, b1> might need swapping
                }
                
            }
        }
    }
    
    // remove any triangles that share an edge
    mesh.remove_triangle_sharing(v(0), v(1), v(2));
}

#endif

#endif //DELAUNAY_H
