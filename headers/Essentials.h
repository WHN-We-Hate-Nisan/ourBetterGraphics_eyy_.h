#pragma once
#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <vector>
constexpr float pi = 3.14159f;
constexpr int max_Vertex = 15;

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
struct Vect3{
	T x, y, z;
	//Vect3() :x(0), y(0), z(0) {}
	//Vect3(T x, T y, T z) : x(x), y(y), z(z) {}
	Vect3 operator-() {
		return { -this->x, -this->y, -this->z };
	}
	Vect3 operator+(const Vect3 &right) {
		return { this->x + right.x, this->y + right.y, this->z + right.z };
	}
	Vect3& operator+=(const Vect3& right) {
		this->x += right.x; this->y += right.y; this->z += right.z;
		return *this;
	}
	Vect3 operator-(const Vect3 &right) {
		return { this->x - right.x, this->y - right.y, this->z - right.z };
	}
	Vect3 operator*(const Vect3 &right) {
		//i  j  k 
		//x  y  z
		//rx ry rz
		
		return { this->y * right.z - right.y * this->z,
					 this->z * right.x - right.y * this->x,
					 this->x * right.y - right.x * this->y };
	}
	Vect3& multiplyEach(const Vect3& right) {
		this->x *= right.x; this->y *= right.y; this->z *= right.z;
		return *this;
	}
	T dot(const Vect3& right) {
		return this->x * right.x + this->y * right.y + this->z * right.z;
	}
	Vect3 normalize() {
		float mag = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
		this-> x /= mag;
		this-> y /= mag;
		this-> z /= mag;
		return *this;
	}
}; 
typedef Vect3<float> Vec3;
struct Triangle {
	Vec3 vertex[3];
	
	Triangle operator+(const Vec3 &right) {
		return { vertex[0] + right,	vertex[1] + right, vertex[2] + right };
	}
	Triangle& operator+=(const Vec3 &right) {
		vertex[0] += right;
		vertex[1] += right;
		vertex[2] += right;
		return *this;
	}
	Triangle& multiply(const Vec3 &right){
		vertex[0].multiplyEach(right);
		vertex[1].multiplyEach(right);
		vertex[2].multiplyEach(right);
		return *this;
	}
	Vec3 normal() {
		return ((vertex[1] - vertex[0]) * (vertex[2] - vertex[0])).normalize();
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

void consoleLog(const char*);
void consoleLog(int);
void consoleLogSpace(int);

int Clamp(int, int, int);
void ClrScr();

inline void DrawPixel(int, int, unsigned int, float = 1000);
template<typename T>
void Swap(T &xp, T &yp);
void SortByY(Vec3 arr[max_Vertex], int n=3);
float interPolateDepth(float input1, float input2, float position, float val1, float val2);

void DrawRect(Vect2<int>, Vect2<int>, unsigned int );
void DrawDDALine(Vect2<int>, Vect2<int>, unsigned int);
void DrawDDALine(float, float, float, float);
void DrawDDALine(float, float, float, float, unsigned);
void DrawDDALine(Vec3, Vec3, unsigned int, Vec3 = { 0, 0, 0 });

void DrawBresLine(Vect2<int>, Vect2<int>, unsigned int);
void DrawBresLine(Vect2<float>, Vect2<float>, unsigned int);
void DrawBresLine(Vec3, Vec3, unsigned int, Vec3 = { 0, 0, 0 });
void DrawBresLine(float, float, float, float);
void DrawBresLine(float, float, float, float, unsigned);

void DrawHorizLine(int, int, int, unsigned int, float, Vec3 = {0, 0, 0});

void DrawTriangle(Triangle, unsigned int = 0xffffff);

int getMidX();
int getMidY();