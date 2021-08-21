#pragma once
#include"Essentials.h"
#include "Texture.h"
#include "Mesh.h"

struct Mat4x4 {
	float m[4][4] = { 0 };
	Vec3 MultiplyVector(Vec3& i) {
		Vec3 o;
		o.x = i.x * m[0][0] + i.y * m[1][0] + i.z * m[2][0] + i.w * m[3][0];
		o.y = i.x * m[0][1] + i.y * m[1][1] + i.z * m[2][1] + i.w * m[3][1];
		o.z = i.x * m[0][2] + i.y * m[1][2] + i.z * m[2][2] + i.w * m[3][2];
		o.w = i.x * m[0][3] + i.y * m[1][3] + i.z * m[2][3] + i.w * m[3][3];
		return o;
	}
	Mat4x4 MultiplyMatrix(Mat4x4& i) {
		Mat4x4 mat;
		for (int col = 0; col < 4; col++)
			for (int row = 0; row < 4; row++)
				for (int k = 0; k < 4; k++)
					mat.m[row][col] += m[row][k] * i.m[k][col];
		*this = mat;
		return *this;
	}
	Mat4x4 MultiplyMatrix(Mat4x4& i1, Mat4x4& i2) {
		Mat4x4 mat;
		for (int col = 0; col < 4; col++)
			for (int row = 0; row < 4; row++)
				for (int k = 0; k < 4; k++)
					mat.m[row][col] += i1.m[row][k] * i2.m[k][col];
		*this = mat;
		return *this;
	}
	Triangle& MultiplyTriangle(Triangle& result, Triangle& in) {
		for (int i = 0; i < 3; i++) {
			result.vertex[i].color = in.vertex[i].color;
			result.vertex[i].textureCood = in.vertex[i].textureCood;
			result.vertex[i].normal = in.vertex[i].normal;
			result.vertex[i].intensity = in.vertex[i].intensity;

			result.vertex[i].position = this->MultiplyVector(in.vertex[i].position);
		}
		return result;
	}
	static Mat4x4 MakeIdentity() {
		Mat4x4 mat;
		mat.m[0][0] = 1.0f;
		mat.m[1][1] = 1.0f;
		mat.m[2][2] = 1.0f;
		mat.m[3][3] = 1.0f;
		return mat;
	}
	static Mat4x4 MakeRotationZ(float deg) {
		float rad = deg * pi / 180.0f;
		Mat4x4 mat;
		mat.m[0][0] = cosf(rad);
		mat.m[0][1] = sinf(rad);
		mat.m[1][0] = -sinf(rad);
		mat.m[1][1] = cosf(rad);
		mat.m[2][2] = 1;
		mat.m[3][3] = 1;
		return mat;
		/*
		cos(theeta),	-sin(theeta),	0, 0,
		sin(theeta),	cos(theeta),	0, 0,
		0,				0,				1, 0,
		0,				0,				0, 1
		*/
	}
	static Mat4x4 MakeRotationY(float deg) {
		float rad = -deg * pi / 180.0f;
		Mat4x4 mat;
		mat.m[0][0] = cosf(rad);
		mat.m[0][2] = -sinf(rad);
		mat.m[1][1] = 1;
		mat.m[2][0] = sinf(rad);
		mat.m[2][2] = cosf(rad);
		mat.m[3][3] = 1;
		return mat;
		/*
		cos(theeta),       0,       +sin(theeta),      0,
		0,                 1,       0,                 0,
		-sin(theeta),      0,       cos(theeta),       0,
		0,                 0,       0,                 1
		*/
	}
	static Mat4x4 MakeRotationX(float deg) {
		float rad = -deg * pi / 180.0f;
		Mat4x4 mat;
		mat.m[0][0] = 1;
		mat.m[1][1] = cosf(rad);
		mat.m[1][2] = sinf(rad);
		mat.m[2][1] = -sinf(rad);
		mat.m[2][2] = cosf(rad);
		mat.m[3][3] = 1;
		return mat;
		/*
		1,              0,              0,              0,
		0,              cos(theeta),    -sin(theeta),   0,
		0,              sin(theeta),    cos(theeta),    0,
		0,              0,              0,				1
		*/
	}
	static Mat4x4 MakeScale(float x, float y, float z) {
		Mat4x4 mat;
		mat.m[0][0] = x;
		mat.m[1][1] = y;
		mat.m[2][2] = x;
		mat.m[3][3] = 1;
		return mat;
		/*
		{x,  0,   0,   0 },
		{0,  x,   0,   0 },
		{0,  0,   x,   0 },
		{0,  0,   0,   1 }
		*/
	}
	static Mat4x4 MakeScale(float s) {
		return MakeScale(s, s, s);
		/*
		{s,  0,   0,   0 },
		{0,  s,   0,   0 },
		{0,  0,   s,   0 },
		{0,  0,   0,   1 }
		*/
	}
	static Mat4x4 MakeTranslate(Vec3 e) {
		Mat4x4 mat;
		mat.m[0][0] = 1;
		mat.m[1][1] = 1;
		mat.m[2][2] = 1;
		mat.m[3][0] = e.x;
		mat.m[3][1] = e.y;
		mat.m[3][2] = e.z;
		mat.m[3][3] = 1;
		return mat;
		/*
		{1,  0,   0,   e.x },
		{0,  1,   0,   e.y },
		{0,  0,   1,   e.z },
		{0,  0,   0,   1   }
		*/
	}
	static Mat4x4 MakeTranslate(float x, float y, float z) {
		return MakeTranslate({ x, y, z });
		/*
		{1,  0,   0,   x },
		{0,  1,   0,   y },
		{0,  0,   1,   z },
		{0,  0,   0,   1   }
		*/
	}
	static Mat4x4 MakeOrthographicProjection(bool x, bool y, bool z) {
		Mat4x4 mat;
		mat.m[0][0] = !x;
		mat.m[1][1] = !y;
		mat.m[2][2] = !z;
		mat.m[3][3] = 1;
		return mat;
		/*
		!x,   0,    0,   0,
		0,    !y,   0,   0,
		0,    0,    !z,  0,
		0,    0,    0,   1
		*/
	}
	static Mat4x4 MakeObliqueProjection(float alpha, float theeta) {
		if (alpha == 0 || alpha == 90 || alpha == 180 || alpha == 270 || alpha == 360) alpha++;

		Mat4x4 mat;
		theeta *= pi / 180;
		alpha *= pi / 180;
		float l1 = 1 / tan(alpha);

		mat.m[0][0] = 1;
		mat.m[1][1] = 1;
		mat.m[2][0] = l1 * cos(theeta);
		mat.m[2][1] = l1 * sin(theeta);
		mat.m[3][3] = 1;
		return mat;
		/*
		{1, 0, l1 * cos(theeta), 0},
		{0, 1, l1 * sin(theeta), 0},
		{0, 0, 0,				 0},
		{0, 0, 0,				 1}
		*/
	}
	static Mat4x4 MakeProjection(float fNear = 0.1f, float fFar = 1000.0f, float fFovDegrees = 90.0f, float fAspectRatio = 0) {
		if (fAspectRatio == 0) fAspectRatio = (float)globalBuffer.height / (float)globalBuffer.width;
		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * pi);

		Mat4x4 mat;
		mat.m[0][0] = fAspectRatio * fFovRad;
		mat.m[1][1] = fFovRad;
		mat.m[2][2] = fFar / (fFar - fNear);
		mat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		mat.m[2][3] = 1.0f;
		mat.m[3][3] = 0.0f;
		return mat;
	}
	static Mat4x4 PointAt(Vec3& position, Vec3& target, Vec3& up) {
		//Calculate new Forward (n)
		Vec3 n = (target - position).normalize();

		//Calculate new Up (u)
		Vec3 u = up - (n * up.dot(n));

		//New right direction (v)
		Vec3 v = u * n;

		//Dimensioning and Translation Matrix
		Mat4x4 mat;
		mat.m[0][0] = v.x;			mat.m[0][1] = v.y;			mat.m[0][2] = v.z;
		mat.m[1][0] = u.x;			mat.m[1][1] = u.y;			mat.m[1][2] = u.z;
		mat.m[2][0] = n.x;			mat.m[2][1] = n.y;			mat.m[2][2] = n.z;
		mat.m[3][0] = position.x;	mat.m[3][1] = position.y;	mat.m[3][2] = position.z; mat.m[3][3] = 1.0f;
		return mat;
	}
	//Inverse Rotation or Translation Matrix
	static Mat4x4 LookAtInverse(Mat4x4& i) {
		Mat4x4 mat;
		mat.m[0][0] = i.m[0][0];	mat.m[0][1] = i.m[1][0];	mat.m[0][2] = i.m[2][0]; // v.x, u.x, n.x, 0
		mat.m[1][0] = i.m[0][1];	mat.m[1][1] = i.m[1][1];	mat.m[1][2] = i.m[2][1]; // v.y, u.y, n.y, 0
		mat.m[2][0] = i.m[0][2];	mat.m[2][1] = i.m[1][2];	mat.m[2][2] = i.m[2][2]; // v.z, u.z, n.z, 0

		for (int j = 0; j < 3; j++) {
			mat.m[3][j] = 0.0f;
			for (int k = 0; k < 3; k++)
				mat.m[3][j] -= i.m[3][k] * mat.m[k][j];
		}
		mat.m[3][3] = 1.0f; // (v x p) (u x p) (n x p) 1
		return mat;
		/*
		v.x u.x, n.x, 0
		v.y u.y, n.y, 0
		v.z u.z, n.z, 0
		(v x p) (u x p) (n x p) 1
		*/
	}
};

