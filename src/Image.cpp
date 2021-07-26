#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MALLOC(sz)           malloc(sz)
#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
#define STBI_FREE(p)              free(p)
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"

Image::Image(const char* filename) {
	if(read(filename)) {
		consoleLog("Read %s\n");
		consoleLog(filename);
		size = width*height;
	}
	else {
		consoleLog("Failed to read %s\n");
		consoleLog(filename);
	}
}

Image::Image(int width, int height, int channels) : width(width), height(height), channels(channels) {
	size = width*height;
	pixels = (Color*)malloc(sizeof(Color) * size);
	memset(pixels, 0, sizeof(Color) * size);
}

Image::Image(const Image& img) : Image(img.width, img.height, img.channels) {
	memcpy(pixels, img.pixels, size*sizeof(Color));
}

Image::~Image() {
	stbi_image_free(pixels);
}

bool Image::read(const char* filename) {
	pixels = (Color*)stbi_load(filename, &width, &height, &channels, 4);
	return pixels != NULL;
}

bool Image::write(const char* filename) {
	ImageType type = getFileType(filename);
	int success;
  switch (type) {
    case PNG:
      success = stbi_write_png(filename, width, height, channels, (uint8_t*)pixels, width*channels);
      break;
    case BMP:
      success = stbi_write_bmp(filename, width, height, channels, (uint8_t*)pixels);
      break;
    case JPG:
      success = stbi_write_jpg(filename, width, height, channels, (uint8_t*)pixels, 100);
      break;
    case TGA:
      success = stbi_write_tga(filename, width, height, channels, (uint8_t*)pixels);
      break;
  }
  return success != 0;
}

ImageType Image::getFileType(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if(ext != nullptr) {
		if(strcmp(ext, ".png") == 0) {
			return PNG;
		}
		else if(strcmp(ext, ".jpg") == 0) {
			return JPG;
		}
		else if(strcmp(ext, ".bmp") == 0) {
			return BMP;
		}
		else if(strcmp(ext, ".tga") == 0) {
			return TGA;
		}
	}
	return PNG;
}