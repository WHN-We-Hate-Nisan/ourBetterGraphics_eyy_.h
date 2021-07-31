#pragma once
#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>

//#include <strstream>
//#include <string>
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
float distance(const float& x1, const float& y1, const float& x2, const float& y2);
unsigned int interPolate(const float& input1, const float& input2, const float& position, const unsigned int& val1, const unsigned int& val2);
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
	Vect2 operator-();
	Vect2 operator+(const Vect2& right);
	Vect2& operator+=(const Vect2& right);
	Vect2 operator-(const Vect2& right);
	Vect2& operator-=(const Vect2& right);
	Vect2 operator*(const Vect2& right);
	Vect2& operator*=(const Vect2& right);
	Vect2 operator*(const T& right);
	Vect2& operator*=(const T& right);
	Vect2 operator/(const T& right);
	Vect2& operator/=(const T& right) {
		this->x /= right; this->y /= right;
		return *this;
	}
	Vect2& multiplyEach(const Vect2&);
	static Vect2 multiplyEach(const Vect2&, const Vect2&);
	T dot(const Vect2&);
	static T dot(const Vect2& , const Vect2&);
	T length();
	Vect2& normalize();
	static Vect2 normalize(Vect2 v);
	static Vect2 interpolate(const float& fraction, const Vect2& left, const Vect2& right);
};
template<typename T = float>
struct Vect3 {
	T x=0, y=0, z=0, w=1;
	//Vect3() :x(0), y(0), z(0) {}
	//Vect3(T x, T y, T z) : x(x), y(y), z(z) {}
	Vect3 operator-(){
		return { -this->x, -this->y, -this->z, this->w };
	}
	Vect3 operator+(const Vect3& right);
	Vect3& operator+=(const Vect3& right);
	Vect3 operator-(const Vect3& right);
	Vect3& operator-=(const Vect3& right){
		this->x -= right.x; this->y -= right.y; this->z -= right.z;
		return *this;
	}
	Vect3 operator*(const Vect3& right);
	Vect3 operator*(const T& right);
	Vect3& operator*=(const T& right);
	Vect3 operator/(const T& right);
	Vect3& operator/=(const T& right);
	Vect3& multiplyEach(const Vect3&);
	static Vect3 multiplyEach(const Vect3&, const Vect3&);
	T dot(const Vect3& right);
	static T dot(const Vect3&, const Vect3&);
	T length();
	Vect3& normalize();
	static Vect3 normalize(Vect3 v);
	static Vect3 intersectPlane(Vect3&, Vect3&, Vect3&, Vect3&, float&);
};
typedef Vect2<float> Vec2;
typedef Vect3<float> Vec3;
struct Vertex {
	Vec3 position;
	Vec2 textureCood;
	Vec3 normal;
	float intensity;
	Color color;
};
struct Triangle {
	Vertex vertex[3];

	Triangle operator+(const Vec3&);
	Triangle& operator+=(const Vec3&);
	Triangle operator*(const Vec3&);
	Triangle& operator*=(const Vec3&);
	Triangle operator/(const float&);
	Triangle& operator/=(const float&);
	Vec3 normal();
	Color avgColor();
	Triangle& normalize();
	static int ClipAgainstPlane(Vec3, Vec3, Triangle&, Triangle&, Triangle&);
};

struct Bitmap {
	BITMAPINFO info;
	int width, height;
	void* memory;
};
extern Bitmap globalBuffer;
extern bool globalRunning;

class Texture
{
private:
	Color* m_Colours = nullptr;

public:
	int nWidth = 0;
	int nHeight = 0;

private:
	void Create(int, int);

public:
	Texture() {}
	Texture(int, int);
	Texture(const char*);
	void SetColour(int, int, Color);
	Color GetColour(int, int);
	Color SampleColour(float, float);
	bool Save(const char*);
	bool Load(const char*);
};
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

void DrawHorizLine(int, int, int, Color, Vec3 = { 0, 0, 0 });
void DrawHorizLineShaded(int, int, int, unsigned char, unsigned char, Vec3 = { 0, 0, 0 });
void DrawHorizTexture(float, float, float, float&, float&, float, float, float, float, Texture*);

void DrawTriangle(Triangle&, Color = 0xffffff);
void ColorTriangle(Triangle&, Color, Vec3 = { 0.0f,0.0f,0.0f });
void ShadeTriangle(Triangle&, Vec3 = { 0.0f,0.0f,0.0f });
void TextureTriangle(Triangle&, Texture*);

int getMidX();
int getMidY();