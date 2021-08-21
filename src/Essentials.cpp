#pragma once
#include "Essentials.h"

void consoleLog(const char* out) {
	OutputDebugStringA(out);
}
void consoleLog(float out) {
	char output[100];
	sprintf_s(output, 100, "%f", out);
	OutputDebugStringA(output);
}
void consoleLogSpace(float out) {
	char output[100];
	sprintf_s(output, 100, "%f\n", out);
	OutputDebugStringA(output);
}

int Clamp(int current, int min, int max) {
	return current < min ? min : current > max ? max : current;
}

template<typename T>
void Swap(T& xp, T& yp) {
	T temp = xp;
	xp = yp;
	yp = temp;
}
template<>
void Swap(int& xp, int& yp) {
	int temp = xp;
	xp = yp;
	yp = temp;
}

float distance(const float& x1, const float& y1, const float& x2, const float& y2) {
	return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	//return abs(x2 - x1) + abs(y2 - y1);
}
unsigned int interPolate(const float& input1, const float& input2, const float& position, const unsigned int& val1, const unsigned int& val2) {
	if (input1 == input2) return val1;
	else return val1 + (position - input1) / (input2 - input1) * (val2 - val1);
}
unsigned char interPolate(const float& input1, const float& input2, const float& position, const unsigned char& val1, const unsigned char& val2) {
	if (input1 == input2) return val1;
	else return val1 + (position - input1) / (input2 - input1) * (val2 - val1);
}
float interPolate(const float& input1, const float& input2, const float& position, const float& val1, const float& val2) {
	if (input1 == input2) return val1;
	else return val1 + (position - input1) / (input2 - input1) * (val2 - val1);
}
float interPolate(const float& fraction, const float& val1, const float& val2) {
	return val1 + fraction * (val2 - val1);
}
float interPolate(const float& fraction, const float& val1, const float& input1, const float& input2) {
	return val1 + fraction * (input2 - input1);
}
//auto interPolate = [](float fraction, float val1, float val2) {
//	return val1 + fraction * (val2 - val1);
//};
Bitmap globalBuffer;
bool globalRunning = true;

void ClrScr(Color color) {
	Color* pixel = (Color*)globalBuffer.memory;
	for (int y = 0; y < globalBuffer.height; y++)
		for (int x = 0; x < globalBuffer.width; x++)
			*pixel++ = gibColorInt(color.r, color.g, color.b, color.a);
}
inline void DrawPixel(int x, int y, Color color) {
	if (x < 0 || x >= globalBuffer.width || y < 0 || y >= globalBuffer.height) return;
	*((Color*)globalBuffer.memory + y * globalBuffer.width + x) = gibColorInt(color.r, color.g, color.b, color.a);
}
void DrawImage(Image image, Vect2<int> offset) {
	for (int i = 0; i < image.height; i++)
		for (int j = 0; j < image.width; j++)
			DrawPixel(j + offset.x, i + offset.y, image.pixels[i * image.width + j].color);
}
void SortByY(Vec3 arr[max_Vertex], int n) {
	int i, j;
	for (i = 0; i < n - 1; i++)
		// Last i elements are already in place 
		for (j = 0; j < n - 1 - i; j++)
			if (arr[j].y > arr[j + 1].y) {
				Swap(arr[j], arr[j + 1]);
			}
}
void SortByYIntensity(Vec3 arr[max_Vertex], float vertex[max_Vertex], int n) {
	int i, j;
	for (i = 0; i < n - 1; i++)
		// Last i elements are already in place 
		for (j = 0; j < n - 1 - i; j++)
			if (arr[j].y > arr[j + 1].y) {
				Swap(arr[j], arr[j + 1]);
				Swap(vertex[j], vertex[j + 1]);
			}
}
void SortByYTextures(Vec3 arr[max_Vertex], Vec2 tex[max_Vertex], int n) {
	int i, j;
	for (i = 0; i < n - 1; i++)
		// Last i elements are already in place 
		for (j = 0; j < n - 1 - i; j++)
			if (arr[j].y > arr[j + 1].y) {
				Swap(arr[j], arr[j + 1]);
				Swap(tex[j], tex[j + 1]);
			}
}
void DrawDDALine(Vect2<int> v1, Vect2<int> v2, Color color)
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
void DrawDDALine(Vec3 v1, Vec3 v2, Color color, Vec3 offset) {
	DrawDDALine(Vect2<int>{(int)(v1.x + offset.x), (int)(v1.y + offset.y)}, Vect2<int>{(int)(v2.x + offset.x), (int)(v2.y + offset.y)}, color);
}

void DrawBresLine(Vect2<int> v1, Vect2<int> v2, Color color) {
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
void DrawBresLine(Vect2<float> v1, Vect2<float> v2, Color color) {
	DrawBresLine(Vect2<int>{(int)v1.x, (int)v1.y}, Vect2<int>{(int)v2.x, (int)v2.y}, color);
}
void DrawBresLine(Vec3 v1, Vec3 v2, Color color, Vec3 offset) {
	DrawBresLine(Vect2<int>{(int)(v1.x + offset.x), (int)(v1.y + offset.y)}, Vect2<int>{(int)(v2.x + offset.x), (int)(v2.y + offset.y)}, color);
}
void DrawBresLine(float x1, float y1, float x2, float y2) {
	DrawBresLine(Vect2<int>{ (int)x1, (int)y1 }, Vect2<int>{ (int)x2, (int)y2 }, 0xffffff);
}
void DrawBresLine(float x1, float y1, float x2, float y2, Color color) {
	DrawBresLine(Vect2<int>{ (int)x1, (int)y1 }, Vect2<int>{ (int)x2, (int)y2 }, color);
}

int getMidX() {
	return globalBuffer.width / 2;
}
int getMidY() {
	return globalBuffer.height / 2;
}