#pragma once
#include"Essentials.h"
#include "Texture.h"

struct Vertex {
	Vec3 position;
	Vec2 textureCood;
	Vec3 normal;
	float intensity;
	Color color;
};
struct Triangle {
	Vertex vertex[3];

	Triangle operator+(const Vec3& right) {
		Triangle tri(*this);
		tri.vertex[0].position += right;
		tri.vertex[1].position += right;
		tri.vertex[2].position += right;
		return tri;
	}
	Triangle& operator+=(const Vec3& right) {
		this->vertex[0].position += right;
		this->vertex[1].position += right;
		this->vertex[2].position += right;
		return *this;
	}
	Triangle operator*(const Vec3& right) {
		Triangle tri(*this);
		tri.vertex[0].position.multiplyEach(right);
		tri.vertex[1].position.multiplyEach(right);
		tri.vertex[2].position.multiplyEach(right);
		return tri;
	}
	Triangle& operator*=(const Vec3& right) {
		this->vertex[0].position.multiplyEach(right);
		this->vertex[1].position.multiplyEach(right);
		this->vertex[2].position.multiplyEach(right);
		return *this;
	}
	Triangle operator/(const float& right) {
		Triangle tri(*this);
		tri.vertex[0].position /= right;
		tri.vertex[1].position /= right;
		tri.vertex[2].position /= right;
		return tri;
	}
	Triangle& operator/=(const float& right) {
		this->vertex[0].position /= right;
		this->vertex[1].position /= right;
		this->vertex[2].position /= right;
		return *this;
	}
	Vec3 normal() {
		return ((vertex[1].position - vertex[0].position) * (vertex[2].position - vertex[0].position)).normalize();
	}
	Color avgColor() {
		return  (vertex[0].color / 3 + vertex[1].color / 3 + vertex[2].color / 3);
	}
	Triangle& normalize() {
		for (int i = 0; i < 3; i++)
			vertex[i].position /= vertex[i].position.w;
		return *this;
	}
};
int ClipAgainstPlane(Vec3, Vec3, Triangle&, Triangle&, Triangle&);
void DrawTriangle(Triangle&, Color = 0xffffff);
void ColorTriangle(Triangle&, Color, Vec3 = { 0.0f,0.0f,0.0f });
void ShadeTriangle(Triangle&, Vec3 = { 0.0f,0.0f,0.0f });
void TextureTriangle(Triangle&, Texture*);
void DrawHorizLine(int, int, int, Color, Vec3 = { 0, 0, 0 });
void DrawHorizLineShaded(int, int, int, Triangle, Vec3 = { 0, 0, 0 });
//void DrawHorizLineShaded(int, int, int, unsigned char, unsigned char, Vec3 );
//void DrawHorizTexture(float, float, float, float&, float&, float, float, float, float, Texture*);
void DrawHorizTexture(float, float, float, float&, float&, float&, float, float, float, float,
	float, float, Texture*);