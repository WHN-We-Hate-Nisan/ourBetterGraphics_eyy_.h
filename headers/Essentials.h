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
#include "Image.h"

constexpr float pi = 3.14159f;
constexpr int max_Vertex = 15;

void consoleLog(const char*);
void consoleLog(int);
void consoleLogSpace(int);

int Clamp(int, int, int);

template<typename T>
void Swap(T &xp, T &yp);
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
	Vect2 operator-(const Vect2& right) {
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
	Vect2& multiplyEach(const Vect2& right) {
		this->x *= right.x; this->y *= right.y;
		return *this;
	}
	static Vect2 multiplyEach(const Vect2& left, const Vect2& right) {
		return { left.x * right.x, left.y * right.y, 1 };
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
	static Vect2 interpolate(const float& fraction,const Vect2& left, const Vect2& right) {
		return { interPolate(fraction, left.u, right.u), interPolate(fraction, left.v, right.v) };
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
		return { left.x * right.x, left.y* right.y, left.z* right.z, 1};
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
	static Vect3 intersectPlane(Vect3& planeP, Vect3& planeN, Vect3& lineStart, Vect3& lineEnd, float &t) {
		planeN.normalize();
		float planeD = -planeN.dot(planeP);
		float ad = lineStart.dot(planeN);
		float bd = lineEnd.dot(planeN);
		t = (planeD + ad) / (ad - bd);

		Vect3 lineStartToEnd = lineEnd - lineStart;
		Vect3 lineToIntersect = lineStartToEnd * t;
		return lineStart + lineToIntersect;
	}
};
typedef Vect2<float> Vec2;
typedef Vect3<float> Vec3;
struct Triangle {
	Vec3 vertex[3];
	Vec2 texCood[3];
	Color color;

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
	Triangle& normalize() {
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
		Vec2* insideTextures[3]; int nInsideTextureCount = 0;
		Vec2* outsideTextures[3]; int nOutsideTextureCount = 0;


		//Get signed distance of each point in triangle to plane
		float dis[3];
		for (int i = 0; i < 3; i++)
			dis[i] = d(in.vertex[i]);
		for (int i = 0; i < 3; i++) {
			if (dis[i] >= 0) {
				insides[nInsidePointCount++] = &in.vertex[i];
				insideTextures[nInsideTextureCount++] = &in.texCood[i];
			}
			else {
				outsides[nOutsidePointCount++] = &in.vertex[i];
				outsideTextures[nOutsideTextureCount++] = &in.texCood[i];
			}
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
				out1.texCood[0] = *insideTextures[0];

				//Two new points, at the location where
				//original sides of the triangle intersect the plane
				float t;
				out1.vertex[1] = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[0], t);
				//out1.texCood[1]=Vec2::interpolate(t, *insideTextures[0], *outsideTextures[0]);
				out1.texCood[1].u = interPolate(t, insideTextures[0]->u, outsideTextures[0]->u);
				out1.texCood[1].v = interPolate(t, insideTextures[0]->v, outsideTextures[0]->v);

				out1.vertex[2] = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[1], t);
				out1.texCood[2].u = interPolate(t, insideTextures[0]->u, outsideTextures[1]->u);
				out1.texCood[2].v = interPolate(t, insideTextures[0]->v, outsideTextures[1]->v);

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
				out1.texCood[0] = *insideTextures[0];
				out1.texCood[1] = *insideTextures[1];
				
				float t;
				out1.vertex[2] = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[0],t);
				out1.texCood[2].u = interPolate(t, insideTextures[0]->u, outsideTextures[0]->u);
				out1.texCood[2].v = interPolate(t, insideTextures[0]->v, outsideTextures[0]->v);

				//Second triangle consists of 1 inside point and
				//location where other side intersects the plane
				//and the newly created point;
				out2.vertex[0] = *insides[1];
				out2.texCood[0] = *insideTextures[1];

				out2.vertex[1] = out1.vertex[2];
				out2.texCood[1] = out1.texCood[2];

				out2.vertex[2] = Vec3::intersectPlane(planeP, planeN, *insides[1], *outsides[0],t);
				out2.texCood[2].u = interPolate(t, insideTextures[1]->u, outsideTextures[0]->u);
				out2.texCood[2].v = interPolate(t, insideTextures[1]->v, outsideTextures[0]->v);

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
	void Create(int width, int height);

public:
	Texture() {}
	Texture(int width, int height);
	Texture(const char* sFile);
	void SetColour(int x, int y, Color c);
	Color GetColour(int x, int y);
	Color SampleColour(float x, float y);
	bool Save(const char* sFile);
	bool Load(const char* sFile);
};
void ClrScr(Color = 0);
inline void DrawPixel(int x, int y, Color color);
void DrawImage(Image, Vect2<int> = { 0,0,1 });
void SortByY(Vec3[max_Vertex], int = 3);
void SortByYTextures(Vec3[max_Vertex], Vec2[max_Vertex], int = 3);
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
void DrawHorizTexture(float, float, float, float&, float&, float, float, float, float, Texture*);

void DrawTriangle(Triangle&, Color = 0xffffff);
void ColorTriangle(Triangle&, Color, Vec3 = { 0.0f,0.0f,0.0f });
void TextureTriangle(Triangle&, Texture*);

int getMidX();
int getMidY();