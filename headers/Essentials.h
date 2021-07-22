#pragma once
#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <strstream>
#include <string>
#include <algorithm>
#include <list>

constexpr float pi = 3.14159f;
constexpr int max_Vertex = 15;

void consoleLog(const char*);
void consoleLog(int);
void consoleLogSpace(int);

int Clamp(int, int, int);

template<typename T>
void Swap(T &xp, T &yp);
unsigned int interPolate(float input1, float input2, float position, unsigned int val1, unsigned int val2);
float interPolate(float input1, float input2, float position, float val1, float val2);

template<typename T = int>
struct Vect2 {
	T x, y;
	Vect2() :x(0), y(0) {}
	Vect2(T x, T y) : x(x), y(y) {}
	Vect2 operator-() {
		return Vect2(-this->x, -this->y);
	}
	Vect2 operator+(Vect2& right) {
		return Vect2(this->x + right.x, this->y + right.y);
	}
	Vect2 operator-(Vect2& right) {
		return Vect2(this->x - right.x, this->y - right.y);
	}
};
template<typename T = float>
struct Vect3 {
	T x=0, y=0, z=0, w=1;
	//Vect3() :x(0), y(0), z(0) {}
	//Vect3(T x, T y, T z) : x(x), y(y), z(z) {}
	Vect3 operator-() {
		return { -this->x, -this->y, -this->z, this->w };
	}
	Vect3 operator+(const Vect3& right) {
		return { this->x + right.x, this->y + right.y, this->z + right.z, this->w };
	}
	Vect3& operator+=(const Vect3& right) {
		this->x += right.x; this->y += right.y; this->z += right.z;
		return *this;
	}
	Vect3 operator-(const Vect3& right) {
		return { this->x - right.x, this->y - right.y, this->z - right.z, this->w };
	}
	Vect3& operator-=(const Vect3& right) {
		this->x -= right.x; this->y -= right.y; this->z -= right.z;
		return *this;
	}
	Vect3 operator*(const Vect3& right) {
		//i  j  k 
		//x  y  z
		//rx ry rz

		return { this->y * right.z - right.y * this->z,
				 this->z * right.x - right.y * this->x,
				 this->x * right.y - right.x * this->y,
				 this->w };
	}
	Vect3 operator*(const float& right) {
		return { this->x * right, this->y * right, this->z * right, this->w };
	}
	Vect3 operator/(const float& right) {
		return { this->x / right, this->y / right, this->z / right, this->w };
	}
	Vect3& operator/=(const float& right) {
		this->x /= right; this->y /= right; this->z /= right;
		return *this;
	}
	Vect3& multiplyEach(const Vect3& right) {
		this->x *= right.x; this->y *= right.y; this->z *= right.z;
		return *this;
	}
	static Vect3 multiplyEach(const Vect3& left, const Vect3& right) {		
		return { left.x * right.x, left.y* right.y, left.z* right.z, 1};
	}
	unsigned int toUnsigned() {
		Vect3<int> Col = { this->x * 256, this->y * 256, this->z * 256 };
		return interPolate(
			(unsigned int)0,
			0xff * 0x100 * 0x100 +
			0xff * 0x100 +
			0xff,
			(unsigned int)Col.x * 0x100 * 0x100 +
			(unsigned int)Col.y * 0x100 +
			(unsigned int)Col.z,
			(unsigned int)0x000000,
			(unsigned int)0xffffff);
	}
	T dot(const Vect3& right) {
		return this->x * right.x + this->y * right.y + this->z * right.z;
	}
	static T dot(const Vect3& left, const Vect3& right) {
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}
	T length() {
		return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	Vect3& normalize() {
		float mag = length();
		this->x /= mag;
		this->y /= mag;
		this->z /= mag;
		return *this;
	}
	static Vect3 normalize(Vect3 v) {
		return v.normalize();
	}
	static Vect3 intersectPlane(Vect3& planeP, Vect3& planeN, Vect3& lineStart, Vect3& lineEnd) {
		planeN.normalize();
		float planeD = -planeN.dot(planeP);
		float ad = lineStart.dot(planeN);
		float bd = lineEnd.dot(planeN);
		float t = (planeD + ad) / (ad - bd);

		Vect3 lineStartToEnd = lineEnd - lineStart;
		Vect3 lineToIntersect = lineStartToEnd * t;
		return lineStart + lineToIntersect;
	}
};
typedef Vect3<float> Vec3;
struct Triangle {
	Vec3 vertex[3];
	unsigned int color;

	Triangle operator+(const Vec3& right) {
		return { vertex[0] + right,	vertex[1] + right, vertex[2] + right };
	}
	Triangle& operator+=(const Vec3& right) {
		vertex[0] += right;
		vertex[1] += right;
		vertex[2] += right;
		return *this;
	}
	Triangle operator*(const Vec3& right) {
		return { 
			vertex[0].multiplyEach(right),
			vertex[1].multiplyEach(right),
			vertex[2].multiplyEach(right)
		};
	}
	Triangle& operator*=(const Vec3& right) {
		vertex[0].multiplyEach(right);
		vertex[1].multiplyEach(right);
		vertex[2].multiplyEach(right);
		return *this;
	}
	Triangle operator/(const float& right) {
		return {
			vertex[0]/right,
			vertex[1]/right,
			vertex[2]/right
		};
	}
	Triangle& operator/=(const float& right) {
		vertex[0] /= right;
		vertex[1] /= right;
		vertex[2] /= right;
		return *this;
	}
	Vec3 normal() {
		return ( (vertex[1] - vertex[0]) * (vertex[2] - vertex[0]) ).normalize();
	}
	Triangle& normalise() {
		for (int i = 0; i < 3; i++)
			vertex[i] /= vertex[i].w;
		return *this;
	}
	static int ClipAgainstPlane(Vec3 planeP, Vec3 planeN, Triangle& in, Triangle& out1, Triangle& out2) {
		//Make plane Normal normal
		planeN.normalize();

		//Return signed shortest distance from point to plane
		auto d = [&](Vec3& p) {
			Vec3 n = Vec3::normalize(p);
			return Vec3::dot(planeN, p) - Vec3::dot(planeN, planeP);
		};

		//Create two storages to classify points on either side of the plane
		//+ve sign = inside
		Vec3* insides[3]; int nInsidePointCount = 0;
		Vec3* outsides[3]; int nOutsidePointCount = 0;
		//Get signed distance of each point in triangle to plane
		float dis[3];
		for (int i = 0; i < 3; i++)
			dis[i] = d(in.vertex[i]);
		for (int i = 0; i < 3; i++) {
			if (dis[i] >= 0) insides[nInsidePointCount++] = &in.vertex[i];
			else outsides[nOutsidePointCount++] = &in.vertex[i];
		}

		//Classify triangle points and change input into
		//smaller triangles if needed

		switch (nInsidePointCount) {
		case 0:
			//All points lie on the outside of the plane
			//Clip whole triangle
			//return nothing
			return 0;
			break;
		case 3:
			//All points lie on the inside of the plane
			//No clipping
			out1 = in;
			return 1;
			break;
		case 1:
			if (nOutsidePointCount == 2) {
				//2 points lie outside
				//triangle becomes smaller triangle
				
				//Set Output
				out1.color = in.color;
				
				out1.vertex[0] = *insides[0];
				//Two new points, at the location where
				//original sides of the triangle intersect the plane
				out1.vertex[1] = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[0]);
				out1.vertex[2] = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[1]);
				return 1;
			}
			break;
		case 2:
			if (nOutsidePointCount == 1) {
				//1 point lies outside
				//triangle becomes a quad

				//Set Output
				out1.color = in.color;
				out2.color = in.color;

				//First triangle consists of 2 inside points and
				//location where one side intersects the plane
				out1.vertex[0] = *insides[0];
				out1.vertex[1] = *insides[1];
				out1.vertex[2] = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[0]);

				//Second triangle consists of 1 inside point and
				//location where other side intersects the plane
				//and the newly created point;
				out2.vertex[0] = *insides[1];
				out2.vertex[1] = out1.vertex[2];
				out2.vertex[2] = Vec3::intersectPlane(planeP, planeN, *insides[1], *outsides[0]);
				return 2;
			}
			break;
		}
		return 0;
	}
};
struct Bitmap {
	BITMAPINFO info;
	int width, height;
	void* memory;
	void* depthBuffer;
};
extern Bitmap globalBuffer;
extern bool globalRunning;

void ClrScr(unsigned int = 0);
inline void DrawPixel(int, int, unsigned int, float = 1000);
void SortByY(Vec3 arr[max_Vertex], int n = 3);
void DrawDDALine(Vect2<int>, Vect2<int>, unsigned int);
void DrawDDALine(float, float, float, float);
void DrawDDALine(float, float, float, float, unsigned);
void DrawDDALine(Vec3, Vec3, unsigned int, Vec3 = { 0, 0, 0 });

void DrawBresLine(Vect2<int>, Vect2<int>, unsigned int);
void DrawBresLine(Vect2<float>, Vect2<float>, unsigned int);
void DrawBresLine(Vec3, Vec3, unsigned int, Vec3 = { 0, 0, 0 });
void DrawBresLine(float, float, float, float);
void DrawBresLine(float, float, float, float, unsigned);

void DrawHorizLine(int, int, int, unsigned int, float = 1000, Vec3 = { 0, 0, 0 });

void DrawTriangle(Triangle, unsigned int = 0xffffff);
void ColorTriangle(Triangle, unsigned int, Vec3 = { 0.0f,0.0f,0.0f });

int getMidX();
int getMidY();