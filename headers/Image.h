#pragma once
#include <stdint.h>
#define gibColorInt(r,g,b,a) (a<<24 | r<<16 | g<<8 | b)

void consoleLog(const char* out);

struct Color
{
	union {
		struct {
			unsigned int color;
		};
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
	};
	Color(unsigned int color=0) :color(color) {}
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :r(r), g(g), b(b), a(a) {}
	Color operator+(Color right) {
		return Color(r + right.r, g + right.g, b + right.b, a + right.a);
	}
	Color& operator+=(Color right) {
		this->r += right.r;
		this->g += right.g;
		this->b += right.b;
		this->a += right.a;
		return *this;
	}
	Color operator-(Color right) {
		return color - right.color;
	}
	Color& operator-=(Color right) {
		this->color -= right.color;
		return *this;
	}
	Color operator*(int right) {
		return Color(r * right, g * right, b * right, a * right);
	}
	Color& operator*=(int right) {
		this->r *= right;
		this->g *= right;
		this->b *= right;
		this->a *= right;
		return *this;
	}
	Color operator/(int right) {
		return Color(r / right, g / right, b / right, a / right);
	}
	Color& operator/=(int right) {
		this->r /= right;
		this->g /= right;
		this->b /= right;
		this->a /= right;
		return *this;
	}
};
enum ImageType {
	PNG, JPG, BMP, TGA
};

struct Image {
	Color* pixels = NULL;
	size_t size = 0;
	int width;
	int height;
	int channels;

	Image(const char* filename);
	Image(int width, int height, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	ImageType getFileType(const char* filename);
};
