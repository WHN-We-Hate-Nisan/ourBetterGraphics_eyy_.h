#pragma once

struct Triangle {
	Vect3<float> vertex[3];
};
struct Mesh {
	std::vector<Triangle> triangles;
};
struct Mat4x4 {
	float m[4][4] = { 0 };
};
class Shape3D {
	Mesh meshCube;
	float fNear, fFar, fFov, fAspectRatio, fFovRad;
public:
	Shape3D() {
		meshCube.triangles = {
			//South
			{0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f},

			//East
			{1.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 1.0f},
			
			//North
			{1.0f, 0.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f},
			{1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f},

			//West
			{0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f},

			//Top
			{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 0.0f},

			//Bottom
			{1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f},

		};
		
		//Projection Matrix
		fNear = 0.1f;
		fFar = 1000.0f;
		fFov = 90.0f;
		fAspectRatio = (float)globalBuffer.height / (float)globalBuffer.width;
		fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * pi);
	}
	void draw() {
		for (auto tri : meshCube.triangles) {

		}
	}
};

