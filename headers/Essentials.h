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
};
template<typename T = int>
struct Vect3 : public Vect2<T> {
	T z;
	Vect3() :Vect2<T>(0, 0), z(z) {}
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
};
Bitmap globalBuffer;
bool globalRunning = true;

void consoleLog(const char* out) {
	OutputDebugStringA(out);
}
void consoleLog(int out) {
	char output[100];
	sprintf_s(output, 100, "%d", out);
	OutputDebugStringA(output);
}
void consoleLogSpace(int out) {
	char output[100];
	sprintf_s(output, 100, "%d\n", out);
	OutputDebugStringA(output);
}

int Clamp(int current, int min, int max) {
	return current < min ? min : current > max ? max : current;
}
void ClrScr() {
	unsigned int* pixel = (unsigned int*)globalBuffer.memory;

	for (int y = 0; y < globalBuffer.height; y++) {
		for (int x = 0; x < globalBuffer.width; x++) {
			*pixel++ = 0;
		}
	}
}

inline void DrawPixel(int x, int y, unsigned int color) {	
	if (x < 0 || x >= globalBuffer.width || y < 0 || y >= globalBuffer.height) return;
	*((unsigned int*)globalBuffer.memory + y * globalBuffer.width + x) = color;
}
void DrawRect(Vect2<int> v1, Vect2<int> v2, unsigned int color) {
	unsigned int* pixel = (unsigned int*)globalBuffer.memory + v1.y * globalBuffer.height + v1.x;

	for (int y = v1.y; y < v2.y; y++) {
		for (int x = v1.x; x < v2.x; x++) {
			*pixel++ = color;
		}
		pixel += globalBuffer.width - (v2.x - v1.x);
	}
}

void DrawDDALine(Vect2<int> v1, Vect2<int> v2, unsigned int color)
{
	if (v1.x < 0 || v1.x >= globalBuffer.width || v1.y < 0 || v1.y >= globalBuffer.height) {
		consoleLog("\nOut OF Bounds: \t");
		consoleLog("x:"); consoleLogSpace(v1.x);
		consoleLog("y:"); consoleLogSpace(v1.y);
		return;
	}
	if (v2.x < 0 || v2.x >= globalBuffer.width || v2.y < 0 || v2.y >= globalBuffer.height) {
		consoleLog("\nOut OF Bounds: \t");
		consoleLog("x:"); consoleLogSpace(v2.x);
		consoleLog("y:"); consoleLogSpace(v2.y);
		return;
	}
	int dx, dy, steps;
	float xincr, yincr, x, y;

	dx = v2.x - v1.x;
	dy = v2.y - v1.y;

	if (abs(dx) >= abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);

	xincr = (float)dx / steps;
	yincr = (float)dy / steps;

	x = v1.x;
	y = v1.y;

	for (int k = 1; k <= steps; k++)
	{
		DrawPixel((int)(x + 0.5f), (int)(y + 0.5f), color);
		x += xincr;
		y += yincr;
	}
}
void DrawDDALine(float x1, float y1, float x2, float y2) {
	DrawDDALine(Vect2<int>{ (int)x1, (int)y1 }, Vect2<int>{ (int)x2, (int)y2 }, 0xffffff);
}
void DrawDDALine(float x1, float y1, float x2, float y2, unsigned color) {
	DrawDDALine(Vect2<int>{ (int)x1, (int)y1 }, Vect2<int>{ (int)x2, (int)y2 }, color);
}
void DrawDDALine(Vect3<float> v1, Vect3<float> v2, unsigned int color, Vect3<float> offset = Vect3<float>(0, 0, 0)) {
	DrawDDALine(Vect2<int>(v1.x + offset.x, v1.y + offset.y), Vect2<int>(v2.x + offset.x, v2.y + offset.y), color);
}

void DrawBresLine(Vect2<int> v1, Vect2<int> v2, unsigned int color) {
	/*if (v1.x < 0 || v1.x >= globalBuffer.width || v1.y < 0 || v1.y >= globalBuffer.height) {
		consoleLog("\nOut OF Bounds: \t");
		consoleLog("x:"); consoleLogSpace(v1.x);
		consoleLog("y:"); consoleLogSpace(v1.y);
		return;
	}
	if (v2.x < 0 || v2.x >= globalBuffer.width || v2.y < 0 || v2.y >= globalBuffer.height) {
		consoleLog("\nOut OF Bounds: \t");
		consoleLog("x:"); consoleLogSpace(v2.x);
		consoleLog("y:"); consoleLogSpace(v2.y);
		return;
	}*/
	//Bresengham's Algorithm
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = v2.x - v1.x;
	dy = v2.y - v1.y;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (v2.x < v1.x) incx = -1;
	incy = 1;
	if (v2.y < v1.y) incy = -1;
	x = v1.x; y = v1.y;
	if (dx > dy) {
		DrawPixel(x, y, color);
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++) {
			if (e >= 0) {
				y += incy;
				e += inc1;
			}
			else
				e += inc2;
			x += incx;
			DrawPixel(x, y, color);
		}

	}
	else {
		DrawPixel(x, y, color);
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++) {
			if (e >= 0) {
				x += incx;
				e += inc1;
			}
			else
				e += inc2;
			y += incy;
			DrawPixel(x, y, color);
		}
	}
}
void DrawBresLine(Vect2<float> v1, Vect2<float> v2, unsigned int color) {
	DrawBresLine(Vect2<int>(v1.x, v1.y), Vect2<int>(v2.x, v2.y), color);
}
void DrawBresLine(Vect3<float> v1, Vect3<float> v2, unsigned int color, Vect3<float> offset = Vect3<float>(0, 0, 0)) {
	DrawBresLine(Vect2<int>(v1.x + offset.x, v1.y + offset.y), Vect2<int>(v2.x + offset.x, v2.y + offset.y), color);
}
void DrawBresLine(float x1, float y1, float x2, float y2) {
	DrawBresLine(Vect2<int>{ (int)x1, (int)y1 }, Vect2<int>{ (int)x2, (int)y2 }, 0xffffff);
}
void DrawBresLine(float x1, float y1, float x2, float y2, unsigned color) {
	DrawBresLine(Vect2<int>{ (int)x1, (int)y1 }, Vect2<int>{ (int)x2, (int)y2 }, color);
}

int getMidX() {
	return globalBuffer.width / 2;
}
int getMidY() {
	return globalBuffer.height / 2;
}