#pragma once

struct Mesh {
	std::vector<Triangle> triangles;
};
struct Mat4x4 {
	float m[4][4] = { 0 };
	static void Multiply(Vec3& i, Vec3& o, Mat4x4& m) {
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
	Vec3 Camera;
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
			{1.0f, 0.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f},
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
	void draw(float fElapsedTime = 0) {
		Mat4x4 matRotZ, matRotX;
		Camera = { 0.0f, 0.0f, 0.0f };
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

			//Rotate in X and Z
			for (int i = 0; i < 3; i++)
				Mat4x4::Multiply(tri.vertex[i], triRotatedZ.vertex[i], matRotZ);
			for (int i = 0; i < 3; i++)
				Mat4x4::Multiply(triRotatedZ.vertex[i], triRotatedZX.vertex[i], matRotX);

			//offset into screen
			triTranslated = triRotatedZX + Vec3{ 0.0f, 0.0f, 3.0f };

			//Find Normal
			Vec3 normal = triTranslated.normal();
			
			if(normal.dot(triTranslated.vertex[0]-Camera) < 0.0f){
				//Illumination
				Vec3 lightDirection = { 0.0f, 0.0f, -1.0f };
				lightDirection.normalize();

				float light = normal.dot(lightDirection);

				unsigned col = interPolate(-1.0f, 1.0f, light, (unsigned int)0, (unsigned int)0xff);
				triTranslated.color 
					= col * 0x100 * 0x100 +
					  col * 0x100 +
					  col;

				//Projection
				for (int i = 0; i < 3; i++)
					Mat4x4::Multiply(triTranslated.vertex[i], triProjected.vertex[i], matProj);

				//Scale Triangles into view
				triProjected += { 1.0f, 1.0f, 0.0f };
				triProjected.multiply({ 0.5f * (float)globalBuffer.width, 0.5f * (float)globalBuffer.height, 1.0f });
				triProjected.color = triTranslated.color;

				colorTriangle(triProjected, triProjected.color);
				//DrawTriangle(triProjected);
			}
		}
	}
	void colorTriangle(Triangle tri, unsigned int color, Vec3 off = { 0.0f,0.0f,0.0f }) {
		float dx1, dx2, dx3;
		Vec3 array[] = { tri.vertex[0], tri.vertex[1], tri.vertex[2] };
		SortByY(array);
		Vec3 A = array[0];
		Vec3 B = array[1];
		Vec3 C = array[2];
		Vec3 Source, End;
		if (B.y - A.y > 0) dx1 = (B.x - A.x) / (B.y - A.y); else dx1 = 0;
		if (C.y - A.y > 0) dx2 = (C.x - A.x) / (C.y - A.y); else dx2 = 0;
		if (C.y - B.y > 0) dx3 = (C.x - B.x) / (C.y - B.y); else dx3 = 0;

		Source = End = A;
		float depthh = 1000;
		if (dx1 > dx2) {
			for (; Source.y <= B.y; Source.y++, End.y++, Source.x += dx2, End.x += dx1)
				DrawHorizLine(Source.x, End.x, Source.y, color, depthh, off);
			End = B;
			for (; Source.y <= C.y; Source.y++, End.y++, Source.x += dx2, End.x += dx3)
				DrawHorizLine(Source.x, End.x, Source.y, color, depthh, off);
		}
		else {
			for (; Source.y <= B.y; Source.y++, End.y++, Source.x += dx1, End.x += dx2)
				DrawHorizLine(Source.x, End.x, Source.y, color, depthh, off);
			Source = B;
			for (; Source.y <= C.y; Source.y++, End.y++, Source.x += dx3, End.x += dx2)
				DrawHorizLine(Source.x, End.x, Source.y, color, depthh, off);
		}
	}
};

