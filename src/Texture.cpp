#pragma once
#include "Essentials.h"
#include "Texture.h"

Texture::Texture(int width, int height) {
	Create(width, height);
}
Texture::Texture(const char* sFile) {
	if (!Load(sFile))
		Create(8, 8);
}
void Texture::Create(int width, int height) {
	nWidth = width;
	nHeight = height;
	m_Colours = new Color[width * height];
	for (int i = 0; i < width * height; i++)
		m_Colours[i] = rand() % 0xffffffff;
}
void Texture::SetColour(int x, int y, Color c) {
	if (x < 0 || x >= nWidth || y < 0 || y >= nHeight) return;
	else m_Colours[y * nWidth + x] = c;
}
Color Texture::GetColour(int x, int y) {
	if (x < 0 || x >= nWidth || y < 0 || y >= nHeight) return 0;
	else return m_Colours[y * nWidth + x];
}
Color Texture::SampleColour(float x, float y) {
	int sx = (int)(x * (float)nWidth);
	int sy = (int)(y * (float)nHeight - 1.0f);
	if (sx < 0 || sx >= nWidth || sy < 0 || sy >= nHeight) return 0;
	else return m_Colours[sy * nWidth + sx];
}
bool Texture::Save(const char* sFile)
{
	Image texture(nWidth, nHeight, 4);
	memcpy(texture.pixels, m_Colours, sizeof(Color) * texture.size);
	return texture.write(sFile);
}
bool Texture::Load(const char* sFile)
{
	delete[] m_Colours;

	Image texture(sFile);

	nWidth = texture.width;
	nHeight = texture.height;

	Create(nWidth, nHeight);

	m_Colours = (Color*)malloc(sizeof(Color) * texture.size);
	memcpy(m_Colours, texture.pixels, sizeof(Color) * texture.size);
	//m_Colours = texture.pixels;	

	return true;
}
