#pragma once
#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <algorithm>
#include <list>
#include "Image.h"

constexpr float pi = 3.14159f;
constexpr int max_Vertex = 15;

void consoleLog(const char*);
void consoleLog(float);
void consoleLogSpace(float);

int Clamp(int, int, int);

template<typename T>
void Swap(T &xp, T &yp);
template<>
void Swap(int& xp, int& yp);
float distance(const float& x1, const float& y1, const float& x2, const float& y2);
unsigned int interPolate(const float& input1, const float& input2, const float& position, const unsigned int& val1, const unsigned int& val2);
unsigned char interPolate(const float& input1, const float& input2, const float& position, const unsigned char& val1, const unsigned char& val2);
float interPolate(const float& input1, const float& input2, const float& position, const float& val1, const float& val2);
float interPolate(const float& fraction, const float& val1, const float& val2);
float interPolate(const float& fraction, const float& val1, const float& input1, const float& input2);

template<typename T = int>
struct Vect2 {
	union {
		struct {
			T x, y, w; //3rd term for normalizing
		};
		struct {
			T u, v, w;
		};
	};
	Vect2 operator-() {
		return { -this->x, -this->y, this->w };
	}
	Vect2 operator+(const Vect2& right) {
		return { this->x + right.x, this->y + right.y, 1 };
	}
	Vect2& operator+=(const Vect2& right) {
		this->x += right.x; this->y += right.y;
		return *this;
	}
	Vect2 operator-(const Vect2& right)  {
		return { this->x - right.x, this->y - right.y, 1 };
	}
	Vect2& operator-=(const Vect2& right) {
		this->x -= right.x; this->y -= right.y;
		return *this;
	}
	Vect2 operator*(const Vect2& right) {
		return { this->x * right.x, this->y * right.y, 1 };
	}
	Vect2& operator*=(const Vect2& right) {
		this->x *= right.x; this->y *= right.y;
		return *this;
	}
	Vect2 operator*(const T& right) {
		return { this->x * right, this->y * right, this->w };
	}
	Vect2& operator*=(const T& right) {
		this->x *= right; this->y *= right;
		return *this;
	}
	Vect2 operator/(const T& right) {
		return { this->x / right, this->y / right, this->w };
	}
	Vect2& operator/=(const T& right) {
		this->x /= right; this->y /= right;
		return *this;
	}
	Vect2& multiplyEach(const Vect2& right) {
		this->x *= right.x; this->y *= right.y;
		return *this;
	}
	static Vect2 multiplyEach(const Vect2& left, const Vect2& right) {
		return { left.x * right.x, left.y * right.y, 1 };
	}
	T dot(const Vect2& right) {
		return this->x * right.x + this->y * right.y;
	}
	static T dot(const Vect2& left, const Vect2& right) {
		return left.x * right.x + left.y * right.y;
	}
	T length() {
		return sqrtf(this->x * this->x + this->y * this->y);
	}
	Vect2& normalize() {
		float mag = length();
		this->x /= mag;
		this->y /= mag;
		return *this;
	}
	static Vect2 normalize(Vect2 v) {
		return v.normalize();
	}
	static Vect2 interpolate(const float& fraction, const Vect2& left, const Vect2& right) {
		return { interPolate(fraction, left.u, right.u), interPolate(fraction, left.v, right.v) };
	}
};
template<typename T = float>
struct Vect3 {
	T x=0, y=0, z=0, w=1;
	//Vect3() :x(0), y(0), z(0) {}
	//Vect3(T x, T y, T z) : x(x), y(y), z(z) {}
	Vect3 operator-(){
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
	Vect3& operator-=(const Vect3& right){
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
	Vect3 operator*(const T& right) {
		return { this->x * right, this->y * right, this->z * right, this->w };
	}
	Vect3& operator*=(const T& right) {
		this->x *= right; this->y *= right; this->z *= right;
		return *this;
	}
	Vect3 operator/(const T& right) {
		return { this->x / right, this->y / right, this->z / right, this->w };
	}
	Vect3& operator/=(const T& right) {
		this->x /= right; this->y /= right; this->z /= right;
		return *this;
	}
	Vect3& multiplyEach(const Vect3& right) {
		this->x *= right.x; this->y *= right.y; this->z *= right.z;
		return *this;
	}
	static Vect3 multiplyEach(const Vect3& left, const Vect3& right) {
		return { left.x * right.x, left.y * right.y, left.z * right.z, 1 };
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
	static Vect3 intersectPlane(Vect3& planeP, Vect3& planeN, Vect3& lineStart, Vect3& lineEnd, float& t) {
		planeN.normalize();
		float planeD = -planeN.dot(planeP);
		float ad = lineStart.dot(planeN);
		float bd = lineEnd.dot(planeN);
		t = (planeD + ad) / (ad - bd);

		Vect3 lineStartToEnd = lineEnd - lineStart;
		Vect3 lineToIntersect = lineStartToEnd * t;
		return lineStart + lineToIntersect;
	}
	Vect3 multiply(T right) {
		return { this->x * right, this->y * right, this->z * right, this->w };
	}
	Vect3 subtract(Vect3 right) {
		return { this->x - right.x, this->y - right.y, this->z - right.z, this->w };
	}
};
typedef Vect2<float> Vec2;
typedef Vect3<float> Vec3;

struct Bitmap {
	BITMAPINFO info;
	int width, height;
	void* memory;
};
extern Bitmap globalBuffer;
extern bool globalRunning;


void ClrScr(Color = 0);
inline void DrawPixel(int, int, Color);
void DrawImage(Image, Vect2<int> = { 0,0,1 });
void SortByY(Vec3[max_Vertex], int = 3);
void SortByYTextures(Vec3[max_Vertex], Vec2[max_Vertex], int = 3);
void SortByYIntensity(Vec3[max_Vertex], float[max_Vertex], int = 3);
void DrawDDALine(Vect2<int>, Vect2<int>, Color);
void DrawDDALine(float, float, float, float);
void DrawDDALine(float, float, float, float, Color);
void DrawDDALine(Vec3, Vec3, Color, Vec3 = { 0, 0, 0 });

void DrawBresLine(Vect2<int>, Vect2<int>, Color);
void DrawBresLine(Vect2<float>, Vect2<float>, Color);
void DrawBresLine(Vec3, Vec3, Color, Vec3 = { 0, 0, 0 });
void DrawBresLine(float, float, float, float);
void DrawBresLine(float, float, float, float, unsigned);


int getMidX();
int getMidY();