#pragma once

typedef unsigned long DWORD;
typedef unsigned int UINT;

struct TriMeshFace
{
    TriMeshFace() {}
    TriMeshFace(DWORD I0, DWORD I1, DWORD I2)
    {
        I[0] = I0;
        I[1] = I1;
        I[2] = I2;
    }

    DWORD I[3];
};
struct GRIDCELL {
	Vec3 p[8];		//position of each corner of the grid in world space
	float val[8];	//value of the function at this grid corner
};

//given a grid cell, returns the set of triangles that approximates the region where val == 0.
/*
   Given a grid cell and an isolevel, calculate the triangular
   facets required to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
	0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/
int Polygonise(GRIDCELL& Grid, TriMeshFace* Triangles, int& NewVertexCount, Vec3* Vertices);
/*
   Linearly interpolate the position where an isosurface cuts
   an edge between two vertices, each with their own scalar value
*/
static Vec3 VertexInterp(Vec3& p1, Vec3& p2, float valp1, float valp2)
{
    return p1 + (p2.subtract(p1)).multiply(valp1 / (valp1 - valp2));
}