#pragma once

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

