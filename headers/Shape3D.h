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
	float fTheta;

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
		fTheta = 0;
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
	void draw(float fElapsedTime=0) {
		Mat4x4 matRotZ, matRotX;
		//fTheta = 10;
		fTheta += fElapsedTime * 0.01f;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		//Draw Triangles
		for (auto tri : meshCube.triangles) {
			Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			for (int i = 0; i < 3; i++)
				Mat4x4::Multiply(tri.vertex[i], triRotatedZ.vertex[i], matRotZ);
			for (int i = 0; i < 3; i++)
				Mat4x4::Multiply(triRotatedZ.vertex[i], triRotatedZX.vertex[i], matRotX);

			triTranslated = triRotatedZX + Vect3<float>{ 0.0f, 0.0f, 3.0f };
	
			for (int i = 0; i < 3; i++)
				Mat4x4::Multiply(triTranslated.vertex[i], triProjected.vertex[i], matProj);

			//Scale Triangles into view
			triProjected += { 1.0f, 1.0f, 0.0f };			
			triProjected.multiply({ 0.5f * (float)globalBuffer.width, 0.5f * (float)globalBuffer.height, 1.0f });
						
			DrawTriangle(triProjected);
		}
	}
};

