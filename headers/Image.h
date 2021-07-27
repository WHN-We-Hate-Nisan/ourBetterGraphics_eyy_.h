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
