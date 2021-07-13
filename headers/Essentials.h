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
template<typename T = int>
struct Vect3 : public Vect2<T> {
	T z;
	Vect3() :Vect2<T>(0, 0), z(0) {}
	Vect3(T x, T y, T z) : Vect2<T>(x, y), z(z) {}
	Vect3 operator-() {
		return Vect3(-this->x, -this->y, -this->z);
	}
	Vect3 operator+(Vect3 &right) {
		return Vect3(this->x+right.x, this->y + right.y, this->z + right.z);
	}
	Vect3 operator-(Vect3 &right) {
		return Vect3(this->x - right.x, this->y - right.y, this->z - right.z);
	}
	Vect3 operator*(Vect3 &right) {
		//i  j  k 
		//x  y  z
		//rx ry rz
		
		return Vect3(this->y * right.z - right.y * this->z,
					 this->z * right.x - right.y * this->x,
					 this->x * right.y - right.x * this->y);
	}
	void normalize() {
		float mag = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
		this-> x /= mag;
		this-> y /= mag;
		this-> z /= mag;
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

inline void DrawPixel(int, int, unsigned int, float=0);

void Swap(Vect3<float>* xp, Vect3<float>* yp);
void SortByY(Vect3<float>arr[max_Vertex], int n=3);

void DrawRect(Vect2<int>, Vect2<int>, unsigned int );
void DrawDDALine(Vect2<int>, Vect2<int>, unsigned int);
void DrawDDALine(float, float, float, float);
void DrawDDALine(float, float, float, float, unsigned);
void DrawDDALine(Vect3<float>, Vect3<float>, unsigned int, Vect3<float> = Vect3<float>(0, 0, 0));

void DrawBresLine(Vect2<int>, Vect2<int>, unsigned int);
void DrawBresLine(Vect2<float>, Vect2<float>, unsigned int);
void DrawBresLine(Vect3<float>, Vect3<float>, unsigned int, Vect3<float> = Vect3<float>(0, 0, 0));
void DrawBresLine(float, float, float, float);
void DrawBresLine(float, float, float, float, unsigned);

void DrawHorizLine(int, int, int, unsigned int, float);

int getMidX();
int getMidY();