#pragma once
#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <strstream>
#include <string>
#include <algorithm>

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
	T length() {
		return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	Vect3 normalize() {
		float mag = length();
		this->x /= mag;
		this->y /= mag;
		this->z /= mag;
		return *this;
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