#pragma once
#include "Essentials.h"

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
	for (int y = 0; y < globalBuffer.height; y++) 
		for (int x = 0; x < globalBuffer.width; x++) 
			*pixel++ = 0;
	float* zBuffer = (float*)globalBuffer.depthBuffer;
	for (int y = 0; y < globalBuffer.height; y++) 
		for (int x = 0; x < globalBuffer.width; x++) 
			*zBuffer++ = 1000.0f;	
}

void Swap(Vect3<float>* xp, Vect3<float>* yp){
	Vect3<float> temp = *xp;
	*xp = *yp;
	*yp = temp;
}
void SortByY(Vect3<float>arr[max_Vertex], int n){
	int i, j;
	for (i = 0; i < n-1; i++)
		// Last i elements are already in place 
		for (j = 0; j < n - 1 - i; j++)
			if (arr[j].y > arr[j + 1].y)
				Swap(&arr[j], &arr[j + 1]);
}
float interPolateDepth(float input1, float input2, float position, float val1, float val2) {
	if (input1 == input2) return val1;
	else return val1 + (position - input1) / (input2 - input1) * (val2 - val1);
}

inline void DrawPixel(int x, int y, unsigned int color, float depth) {
	if (x < 0 || x >= globalBuffer.width || y < 0 || y >= globalBuffer.height) return;
	float* prevDepth = (float*)globalBuffer.depthBuffer + y * globalBuffer.width + x;
	//if ( *prevDepth < depth) return; else *prevDepth = depth;
	*((unsigned int*)globalBuffer.memory + y * globalBuffer.width + x) = color;
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
void DrawDDALine(Vect3<float> v1, Vect3<float> v2, unsigned int color, Vect3<float> offset) {
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
void DrawBresLine(Vect3<float> v1, Vect3<float> v2, unsigned int color, Vect3<float> offset) {
	DrawBresLine(Vect2<int>(v1.x + offset.x, v1.y + offset.y), Vect2<int>(v2.x + offset.x, v2.y + offset.y), color);
}
void DrawBresLine(float x1, float y1, float x2, float y2) {
	DrawBresLine(Vect2<int>{ (int)x1, (int)y1 }, Vect2<int>{ (int)x2, (int)y2 }, 0xffffff);
}
void DrawBresLine(float x1, float y1, float x2, float y2, unsigned color) {
	DrawBresLine(Vect2<int>{ (int)x1, (int)y1 }, Vect2<int>{ (int)x2, (int)y2 }, color);
}

void DrawHorizLine(int x1, int x2, int y, unsigned int color, float depth, Vect3<float> off) {
	for (int i = x1; i <= x2; i++)
		DrawPixel(i + off.x, y + off.y, color, depth);
}

int getMidX() {
	return globalBuffer.width / 2;
}
int getMidY() {
	return globalBuffer.height / 2;
}
