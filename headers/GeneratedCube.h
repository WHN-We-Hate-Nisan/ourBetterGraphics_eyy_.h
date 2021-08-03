#pragma once
#include "MarchingCubes.h"

float SphereFunction(const Vec3&);
float RandomFunction(const Vec3&);
float CrazyFunction(const Vec3&);
float PlaneFunction(const Vec3&);
double OkByeFunction(Vec3, int = 4, Vec3 = { 0,0,0 }, int = 1);
Color interPolateColors(float input1, float input2, float position, Color Color1, Color Color2);
float Fourier(float f1, float f2);

class GeneratedCube {
private:
    Vertex* mesh;
    static const UINT FunctionCount = 4;
    float (*_functions[FunctionCount])(const Vec3& Pos);
    UINT _functionIndex;

    UINT _XCount, _YCount, _ZCount;
    Vec3 _Start, _End, _Diff;
    float _CellSize;

    std::vector<Vec3> _AllVertices;
    std::vector<TriMeshFace> _AllFaces;

    //Mesh
    UINT _VertexCount, _IndexCount;
    float* _Vertices;
    DWORD* _Indices;

    //Temp Storage
    //Temp Storage
    TriMeshFace _FaceStorage[10];
    Vec3 _VertexStorage[15];

    float (*_Function)(const Vec3& Pos);
    float _Epsilon;

public:
    std::vector<Triangle> triangles;

    GeneratedCube(float boxSize=10.0f, float cellSize=1.0f) {
        _Vertices = 0;
        _Indices = 0;
        _VertexCount = 0;
        _IndexCount = 0;
        _Epsilon = 0.0003f;

        _functionIndex = 3;
        _functions[0] = SphereFunction;
        _functions[1] = RandomFunction;
        _functions[2] = CrazyFunction;
        _functions[3] = PlaneFunction;
        triangles= std::vector<Triangle>();

        IsoApproximate(boxSize, cellSize, _functions[_functionIndex]);
        //IsoApproximate(1.25f, 0.025f, _functions[_functionIndex]);
    }
    //Generate Surface
    void IsoApproximate(float BoxSize, float CellSize, float (*Function)(const Vec3& Pos))
    {
        Vec3 End = { BoxSize, BoxSize, BoxSize, 1.0f };
        IsoApproximate(-End, End, CellSize, Function);
    }
    void IsoApproximate(const Vec3& Start, const Vec3& End, float CellSize, float (*Function)(const Vec3& Pos))
    {
        _Function = Function;
        _CellSize = CellSize;
        _Start = Start;
        _End = End;
        _Diff = _End - _Start;
        _XCount = int(_Diff.x / _CellSize);
        _YCount = int(_Diff.y / _CellSize);
        _ZCount = int(_Diff.z / _CellSize);

        float* TopGrid = new float[_XCount * _YCount];
        float* BottomGrid = new float[_XCount * _YCount];

        FillGrid(TopGrid, 0);
        for (UINT z = 1; z < _ZCount - 1; z++)
        {
            FillGrid(BottomGrid, z);
            PolygonizeGrids(TopGrid, BottomGrid, z);
            Swap(TopGrid, BottomGrid);
        }

        delete[] TopGrid;
        delete[] BottomGrid;

        MakeObject();
    }
    void MakeObject()//Sets up Indices and Vertices
    {
        const UINT noOfVertices = _AllVertices.size();
        AllocateMesh(noOfVertices, _AllFaces.size());

        mesh = new Vertex[noOfVertices];
        for (UINT VertexIndex = 0; VertexIndex < _AllVertices.size(); VertexIndex++)
        {
            Vertex& CurVertex= mesh[VertexIndex];
            CurVertex.position = _AllVertices[VertexIndex];
            CalcGradient(CurVertex.position, CurVertex.normal);            
        }
        for (UINT FaceIndex = 0; FaceIndex < _AllFaces.size(); FaceIndex++)
        {
            const TriMeshFace& CurFace = _AllFaces[FaceIndex];
            _Indices[FaceIndex * 3 + 0] = CurFace.I[0];
            _Indices[FaceIndex * 3 + 1] = CurFace.I[1];
            _Indices[FaceIndex * 3 + 2] = CurFace.I[2];
        }
        ColorbyNormals(mesh);
        //ColorbyInterPolation(meshVertices);
        //ConvertToVertices(mesh);
        FillTriangles(mesh);
    }
    void ConvertToVertices(Vertex* meshVertices) {
        const UINT noOfVertices = _AllVertices.size();
        
		for (UINT VertexIndex = 0; VertexIndex < noOfVertices; VertexIndex++) {
			_Vertices[9 * VertexIndex + 0] = meshVertices[VertexIndex].position.x;
			_Vertices[9 * VertexIndex + 1] = meshVertices[VertexIndex].position.y;
			_Vertices[9 * VertexIndex + 2] = meshVertices[VertexIndex].position.z;
			_Vertices[9 * VertexIndex + 3] = meshVertices[VertexIndex].normal.x;
			_Vertices[9 * VertexIndex + 4] = meshVertices[VertexIndex].normal.y;
			_Vertices[9 * VertexIndex + 5] = meshVertices[VertexIndex].normal.z;
            _Vertices[9 * VertexIndex + 6] = meshVertices[VertexIndex].color.r;
            _Vertices[9 * VertexIndex + 7] = meshVertices[VertexIndex].color.g;
            _Vertices[9 * VertexIndex + 8] = meshVertices[VertexIndex].color.b;
        }
    }
    void ColorbyNormals(Vertex* meshVertices,
        float fr = 256.0f, float fg = 256.0f, float fb = 256.0f)
	{
		for (UINT VertexIndex = 0; VertexIndex < _AllVertices.size(); VertexIndex++) {

			unsigned char r = (meshVertices[VertexIndex].normal.x / 2.0f + 0.5f) * fr;
			unsigned char g = (meshVertices[VertexIndex].normal.y / 2.0f + 0.5f) * fg;
			unsigned char b = (meshVertices[VertexIndex].normal.z / 2.0f + 0.5f) * fb;    //remap each normal's (x, y, z) to (r, g, b)
			meshVertices[VertexIndex].color = Color(r, g, b, 0xff);
		}
	}
    void ColorbyInterPolation(Vertex* meshVertices)
    {
        for (UINT VertexIndex = 0; VertexIndex < _AllVertices.size(); VertexIndex++) {           
            Color ColorA = Color(0.0f, 150.0f, 198.0f, 0xff); //from  0, 150, 199 //Lower Color
            Color ColorB = Color(233.0f, 196.0f, 106.0f, 0xff); //to   233, 196, 106 //Upper Color
            meshVertices[VertexIndex].color = interPolateColors(0.3f, 0.8f, meshVertices[VertexIndex].position.y, ColorA, ColorB);
        }
    }
    void ColorbyThresHold(Vertex* meshVertices)
    {
        for (UINT VertexIndex = 0; VertexIndex < _AllVertices.size(); VertexIndex++) {
            Color ColorL1 = Color(  3.0f,   4.0f,  94.0f, 0xff); //from   3,   4,  94 //Lower Color1
            Color ColorL2 = Color(  0.0f, 150.0f, 198.0f, 0xff); //from   0, 150, 199 //Lower Color2
            Color ColorU1 = Color(108.0f, 197.0f,  81.0f, 0xff); //to   108, 197, 81 //Upper Color1
            Color ColorU2 = Color(233.0f, 196.0f, 106.0f, 0xff); //to   233, 196, 106 //Upper Color2
            float lower = -1.0f, threshold = 0.1f, upper = 1.0f;
            //float lower = 0.2f, threshold = 0.4f, upper = 0.7f;
            //remap each normal's (x, y, z) to (r, g, b)
            if (meshVertices[VertexIndex].position.y < threshold) 
                meshVertices[VertexIndex].color = interPolateColors(lower, threshold, meshVertices[VertexIndex].position.y, ColorL1, ColorL2);
            else 
                meshVertices[VertexIndex].color = interPolateColors(threshold, upper, meshVertices[VertexIndex].position.y, ColorU1, ColorU2);
        }
    }
    template <class type> __forceinline void Swap(type& t1, type& t2)
    {
        type Temp = t1;
        t1 = t2;
        t2 = Temp;
    }
    void FillTriangles(Vertex* meshVertices) {
        for (UINT VertexIndex = 0; VertexIndex < _IndexCount/3; VertexIndex++) {
            Triangle tri;
            for (int i = 0; i < 3; i++) 
                tri.vertex[i] = mesh[_Indices[VertexIndex * 3 + i]];   
            triangles.push_back(tri);
        }
    }
    //Mesh
    void FreeMesh()
    {
        _VertexCount = 0;
        _IndexCount = 0;
        if (_Vertices)
        {
            delete[] _Vertices;
            _Vertices = 0;
        }
        if (_Indices)
        {
            delete[] _Indices;
            _Indices = 0;
        }
    }
    void AllocateMesh(UINT NewVertexCount, UINT NewFaceCount)
    {
        FreeMesh();
        if (NewVertexCount == 0 || NewFaceCount == 0) return;
        _VertexCount = NewVertexCount;
        _IndexCount = NewFaceCount * 3;
        _Vertices = new float[_VertexCount];
        _Indices = new DWORD[_IndexCount];
    }
    void CalcGradient(const Vec3& Pos, Vec3& Normal)
    {
        float value = _Function(Pos);
        Normal.x = (_Function({Pos.x + _Epsilon, Pos.y, Pos.z, 1.0f}) - value);
        Normal.y = (_Function({Pos.x, Pos.y + _Epsilon, Pos.z, 1.0f}) - value);
        Normal.z = (_Function({ Pos.x, Pos.y, Pos.z + _Epsilon, 1.0f }) - value);
        Normal.normalize();
    }
    void FillGrid(float* Grid, int z)
    {
        for (UINT x = 0; x < _XCount; x++)
        {
            for (UINT y = 0; y < _YCount; y++)
            {
                Vec3 Pos = { _Start.x + _CellSize * x, _Start.y + _CellSize * y, _Start.z + _CellSize * (z + 1) , 1.0f};
                Grid[x * _YCount + y] = _Function(Pos);
            }
        }
    }
    bool PushPolygons(GRIDCELL& g)
    {
        int NewVertexCount, IndexShift = _AllVertices.size();
        int NewFaceCount = Polygonise(g, _FaceStorage, NewVertexCount, _VertexStorage);
        if (NewFaceCount)
        {
            for (int FaceIndex = 0; FaceIndex < NewFaceCount; FaceIndex++)
            {
                _FaceStorage[FaceIndex].I[0] += IndexShift;
                _FaceStorage[FaceIndex].I[1] += IndexShift;
                _FaceStorage[FaceIndex].I[2] += IndexShift;
                _AllFaces.push_back(_FaceStorage[FaceIndex]);
            }
            for (int VertexIndex = 0; VertexIndex < NewVertexCount; VertexIndex++)
            {
                _AllVertices.push_back(_VertexStorage[VertexIndex]);
            }
            return true;
        }
        return false;
    }
    void PolygonizeGrids(float* TopVals, float* BottomVals, int z)
    {
        for (UINT x = 0; x < _XCount - 1; x++)
        {
            for (UINT y = 0; y < _YCount - 1; y++)
            {
                GRIDCELL g;
                g.p[0] = {_Start.x + _CellSize * (x + 0), _Start.y + _CellSize * (y + 0), _Start.z + _CellSize * (z + 0), 1.0f };
                g.p[1] = {_Start.x + _CellSize * (x + 1), _Start.y + _CellSize * (y + 0), _Start.z + _CellSize * (z + 0), 1.0f };
                g.p[2] = {_Start.x + _CellSize * (x + 1), _Start.y + _CellSize * (y + 1), _Start.z + _CellSize * (z + 0), 1.0f };
                g.p[3] = {_Start.x + _CellSize * (x + 0), _Start.y + _CellSize * (y + 1), _Start.z + _CellSize * (z + 0), 1.0f };

                g.val[0] = TopVals[x * _YCount + y];
                g.val[1] = TopVals[(x + 1) * _YCount + y];
                g.val[2] = TopVals[(x + 1) * _YCount + y + 1];
                g.val[3] = TopVals[x * _YCount + y + 1];

                g.p[4] = {_Start.x + _CellSize * (x + 0), _Start.y + _CellSize * (y + 0), _Start.z + _CellSize * (z + 1), 1.0f};
                g.p[5] = {_Start.x + _CellSize * (x + 1), _Start.y + _CellSize * (y + 0), _Start.z + _CellSize * (z + 1), 1.0f};
                g.p[6] = {_Start.x + _CellSize * (x + 1), _Start.y + _CellSize * (y + 1), _Start.z + _CellSize * (z + 1), 1.0f};
                g.p[7] = {_Start.x + _CellSize * (x + 0), _Start.y + _CellSize * (y + 1), _Start.z + _CellSize * (z + 1), 1.0f};

                g.val[4] = BottomVals[x * _YCount + y];
                g.val[5] = BottomVals[(x + 1) * _YCount + y];
                g.val[6] = BottomVals[(x + 1) * _YCount + y + 1];
                g.val[7] = BottomVals[x * _YCount + y + 1];

                bool Valid = true;
                for (UINT VertexIndex = 0; VertexIndex < 8 && Valid; VertexIndex++)
                {
                    if (g.val[VertexIndex] == FLT_MAX)
                    {
                        Valid = false;
                    }
                }
                if (Valid)
                {
                    PushPolygons(g);
                }
            }
        }
    }
    //Get Values
    float* getVertices(size_t* size=0) {
        if (size) *size = _VertexCount;
        return _Vertices;
    }
    unsigned int* getIndices(size_t* size = 0) {
        if (size) *size = _IndexCount;
        return (unsigned int*)_Indices;
    }
    ~GeneratedCube() {
        delete[] mesh;
    }
};
