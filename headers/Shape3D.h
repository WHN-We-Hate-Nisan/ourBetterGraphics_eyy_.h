#pragma once

struct Mesh {
	std::vector<Triangle> triangles;
};
struct Mat4x4 {
	float m[4][4] = { 0 };	
	static void Multiply(Vect3<float>& i, Vect3<float>& o, Mat4x4& m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
			o.x /= w; o.y /= w; o.z /= w;
	}
};

class Shape3D {
	Mesh meshCube;
	Mat4x4 matProj;
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

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
	}
	void draw() {
		//Draw Triangles
		for (auto tri : meshCube.triangles) {
			Triangle triProjected, triTranslated;

			triTranslated = tri+ Vect3<float>{ 0.0f, 0.0f, 3.0f };

			for (int i = 0; i < 3; i++)
				Mat4x4::Multiply(triTranslated.vertex[i], triProjected.vertex[i], matProj);

			//Scale Triangles into view
			triProjected += { 1.0f, 1.0f, 0.0f };			
			triProjected.multiply({ 0.5f * (float)globalBuffer.width, 0.5f * (float)globalBuffer.height, 1.0f });
						
			DrawTriangle(triProjected);
		}
	}
};

