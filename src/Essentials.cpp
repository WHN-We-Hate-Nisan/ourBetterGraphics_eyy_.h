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

void DrawHorizLine(int x1, int x2, int y, Color color, Vec3 off) {
	if (x2 < x1)
		Swap(x1, x2);
	for (int i = x1; i <= x2; i++)
		DrawPixel(i + off.x, y + off.y, color);
}
void DrawHorizLineShaded(int x1, int x2, int y, Triangle tri, Vec3 off) {
	if (x2 < x1)
		Swap(x1, x2);
	for (int i = x1; i <= x2; i++) {
		//Barycentric coordinates
		float W1 = ((tri.vertex[1].position.y - tri.vertex[2].position.y) * (i - tri.vertex[2].position.x) + (tri.vertex[2].position.x - tri.vertex[1].position.x) * (y - tri.vertex[2].position.y)) /
			((tri.vertex[1].position.y - tri.vertex[2].position.y) * (tri.vertex[0].position.x - tri.vertex[2].position.x) + (tri.vertex[2].position.x - tri.vertex[1].position.x) * (tri.vertex[0].position.y - tri.vertex[2].position.y));
		float W2 = ((tri.vertex[2].position.y - tri.vertex[0].position.y) * (i - tri.vertex[2].position.x) + (tri.vertex[0].position.x - tri.vertex[2].position.x) * (y - tri.vertex[2].position.y)) /
			((tri.vertex[1].position.y - tri.vertex[2].position.y) * (tri.vertex[0].position.x - tri.vertex[2].position.x) + (tri.vertex[2].position.x - tri.vertex[1].position.x) * (tri.vertex[0].position.y - tri.vertex[2].position.y));
		float W3 = 1 - W1 - W2;

		float a = (W1 * tri.vertex[0].intensity / 12.5f + W2 * tri.vertex[1].intensity / 12.5f + W3 * tri.vertex[2].intensity / 12.5f);
		unsigned char r = W1 * tri.vertex[0].color.r + W2 * tri.vertex[1].color.r + W3 * tri.vertex[2].color.r;
		unsigned char g = W1 * tri.vertex[0].color.g + W2 * tri.vertex[1].color.g + W3 * tri.vertex[2].color.g;
		unsigned char b = W1 * tri.vertex[0].color.b + W2 * tri.vertex[1].color.b + W3 * tri.vertex[2].color.b;

		DrawPixel(i + off.x, y + off.y, Color(r * a, g * a, b * a, 0xff));
	}
}
void DrawHorizTexture(float ax, float bx, float y, float& texU, float& texV, float& texW,
	float texStartU, float texEndU, float texStartV, float texEndV,
	float texStartW, float texEndW, Texture* texture) {
	float tStep = 1.0f / ((float)(bx - ax));
	float t = 0.0f;
	for (int j = ax; j < bx; j++) {
		//interpolate in tex-el space
		texU = (1.0f - t) * texStartU + t * texEndU;
		texV = (1.0f - t) * texStartV + t * texEndV;
		texW = (1.0f - t) * texStartW + t * texEndW;
		DrawPixel(j, (int)y, texture->SampleColour(texU / texW, texV / texW));
		//DrawPixel(j, (int)y, texture->SampleColour(texU, texV));
		t += tStep;
	}
}

void DrawTriangle(Triangle& t, Color color) {
	DrawBresLine(t.vertex[0].position, t.vertex[1].position, color);
	DrawBresLine(t.vertex[1].position, t.vertex[2].position, color);
	DrawBresLine(t.vertex[2].position, t.vertex[0].position, color);
}
void ColorTriangle(Triangle& tri, Color color, Vec3 off) {
	float dx1, dx2, dx3;
	Vec3 array[] = { tri.vertex[0].position, tri.vertex[1].position, tri.vertex[2].position };
	SortByY(array);
	Vec3 A = array[0];
	Vec3 B = array[1];
	Vec3 C = array[2];
	Vec3 Source, End;
	if (B.y - A.y > 0) dx1 = (B.x - A.x) / (B.y - A.y); else dx1 = 0;
	if (C.y - A.y > 0) dx2 = (C.x - A.x) / (C.y - A.y); else dx2 = 0;
	if (C.y - B.y > 0) dx3 = (C.x - B.x) / (C.y - B.y); else dx3 = 0;

	Source = End = A;
	if (dx1 > dx2) {
		for (; Source.y <= B.y; Source.y++, End.y++, Source.x += dx2, End.x += dx1)
			DrawHorizLine(Source.x, End.x, Source.y, color, off);
		End = B;
		for (; Source.y <= C.y; Source.y++, End.y++, Source.x += dx2, End.x += dx3)
			DrawHorizLine(Source.x, End.x, Source.y, color, off);
	}
	else {
		for (; Source.y <= B.y; Source.y++, End.y++, Source.x += dx1, End.x += dx2)
			DrawHorizLine(Source.x, End.x, Source.y, color, off);
		Source = B;
		for (; Source.y <= C.y; Source.y++, End.y++, Source.x += dx3, End.x += dx2)
			DrawHorizLine(Source.x, End.x, Source.y, color, off);
	}
}
void ShadeTriangle(Triangle& tri, Vec3 off) {
	float dx1, dx2, dx3;
	Vec3 array[] = { tri.vertex[0].position, tri.vertex[1].position, tri.vertex[2].position };
	SortByY(array);
	Vec3 A = array[0];
	Vec3 B = array[1];
	Vec3 C = array[2];

	Vec3 Source, End;
	if (B.y - A.y > 0) dx1 = (B.x - A.x) / (B.y - A.y); else dx1 = 0;
	if (C.y - A.y > 0) dx2 = (C.x - A.x) / (C.y - A.y); else dx2 = 0;
	if (C.y - B.y > 0) dx3 = (C.x - B.x) / (C.y - B.y); else dx3 = 0;

	Source = End = A;
	if (dx1 > dx2) {
		for (; Source.y <= B.y; Source.y++, End.y++, Source.x += dx2, End.x += dx1)
			DrawHorizLineShaded(Source.x, End.x, Source.y, tri, off);
		End = B;
		for (; Source.y <= C.y; Source.y++, End.y++, Source.x += dx2, End.x += dx3)
			DrawHorizLineShaded(Source.x, End.x, Source.y, tri, off);
	}
	else {
		for (; Source.y <= B.y; Source.y++, End.y++, Source.x += dx1, End.x += dx2)
			DrawHorizLineShaded(Source.x, End.x, Source.y, tri, off);
		Source = B;
		for (; Source.y <= C.y; Source.y++, End.y++, Source.x += dx3, End.x += dx2)
			DrawHorizLineShaded(Source.x, End.x, Source.y, tri, off);
	}
}
void TextureTriangle(Triangle& tri, Texture* texture) {
	//Sort  Vertices by y value
	Vec3 array[] = { tri.vertex[0].position, tri.vertex[1].position, tri.vertex[2].position };
	Vec2 textureArray[] = { tri.vertex[0].textureCood, tri.vertex[1].textureCood, tri.vertex[2].textureCood };
	SortByYTextures(array, textureArray);
	Vec3 A = array[0];
	Vec3 B = array[1];
	Vec3 C = array[2];
	Vec2 ATex = textureArray[0];
	Vec2 BTex = textureArray[1];
	Vec2 CTex = textureArray[2];

	//Setup variables to find gradients
	float dx1, dx2;
	float dy1, dy2;
	float du1, du2;
	float dv1, dv2;
	float dw1, dw2;

	float texU, texV, texW;

	//Left top line of triangle
	dy1 = B.y - A.y;
	dx1 = B.x - A.x;
	dv1 = BTex.v - ATex.v;
	du1 = BTex.u - ATex.u;
	dw1 = BTex.w - ATex.w;

	//Right top line of triangle
	dy2 = C.y - A.y;
	dx2 = C.x - A.x;
	dv2 = CTex.v - ATex.v;
	du2 = CTex.u - ATex.u;
	dw2 = CTex.w - ATex.w;

	//Amount to move in each step
	float dAxStep = 0, dBxStep = 0,
		dU1Step = 0, dV1Step = 0,
		dU2Step = 0, dV2Step = 0,
		dW1Step = 0, dW2Step = 0;

	if (dy1) dAxStep = dx1 / (float)abs(dy1);
	if (dy2) dBxStep = dx2 / (float)abs(dy2);

	if (dy1) dU1Step = du1 / (float)abs(dy1);
	if (dy1) dV1Step = dv1 / (float)abs(dy1);
	if (dy1) dW1Step = dw1 / (float)abs(dy1);

	if (dy2) dU2Step = du2 / (float)abs(dy2);
	if (dy2) dV2Step = dv2 / (float)abs(dy2);
	if (dy2) dW2Step = dw2 / (float)abs(dy2);

	//Draw top half
	if (dy1) {
		for (float i = A.y; i < B.y; i++) {
			int ax = interPolate(dAxStep, A.x, A.y, i);
			int bx = interPolate(dBxStep, A.x, A.y, i);

			//Calculate texture start point
			float texStartU = interPolate(dU1Step, ATex.u, A.y, i);
			float texStartV = interPolate(dV1Step, ATex.v, A.y, i);
			float texStartW = interPolate(dW1Step, ATex.w, B.y, i);

			//Calculate texture end point
			float texEndU = interPolate(dU2Step, ATex.u, A.y, i);
			float texEndV = interPolate(dV2Step, ATex.v, A.y, i);
			float texEndW = interPolate(dW2Step, ATex.w, B.y, i);

			if (ax > bx) {
				Swap(ax, bx);
				Swap(texStartU, texEndU);
				Swap(texStartV, texEndV);
				Swap(texStartW, texEndW);
			}

			//Final texture point
			texU = texStartU;
			texV = texStartV;
			texW = texStartW;

			//Draw Line
			DrawHorizTexture(ax, bx, i, texU, texV, texW, texStartU, texEndU, texStartV, texEndV, texStartW, texEndW, texture);
		}

		//Left bottom line of triangle
		dy1 = C.y - A.y;
		dx1 = C.x - A.x;
		dv1 = CTex.v - ATex.v;
		du1 = CTex.u - ATex.u;
		dw1 = CTex.w - ATex.w;

		//Amount to move in each step
		if (dy1) dAxStep = dx1 / (float)abs(dy1);
		if (dy2) dBxStep = dx2 / (float)abs(dy2);

		dU1Step = 0; dU2Step = 0;
		if (dy1) dU1Step = du1 / (float)abs(dy1);
		if (dy1) dV1Step = dv1 / (float)abs(dy1);
		if (dy1) dW1Step = dw1 / (float)abs(dy1);

		for (float i = B.y; i < C.y; i++) {
			int ax = interPolate(dAxStep, B.x, B.y, i);
			int bx = interPolate(dBxStep, A.x, A.y, i);

			//Calculate texture start point
			float texStartU = interPolate(dU1Step, BTex.u, B.y, i);
			float texStartV = interPolate(dV1Step, BTex.v, B.y, i);
			float texStartW = interPolate(dW1Step, BTex.w, B.y, i);

			//Calculate texture end point
			float texEndU = interPolate(dU2Step, ATex.u, A.y, i);
			float texEndV = interPolate(dV2Step, ATex.v, A.y, i);
			float texEndW = interPolate(dW2Step, ATex.w, A.y, i);

			if (ax > bx) {
				Swap(ax, bx);
				Swap(texStartU, texEndU);
				Swap(texStartV, texEndV);
				Swap(texStartW, texEndW);
			}

			//Final texture point
			texU = texStartU;
			texV = texStartV;
			texW = texStartW;

			//Draw Line
			DrawHorizTexture(ax, bx, i, texU, texV, texW, texStartU, texEndU, texStartV, texEndV, texStartW, texEndW, texture);
		}
	}
}

int getMidX() {
	return globalBuffer.width / 2;
}
int getMidY() {
	return globalBuffer.height / 2;
}


template <typename T>
Vect2<T> Vect2<T>::operator-() {
	return { -this->x, -this->y, this->w };
}

template <typename T>
Vect2<T> Vect2<T>::operator+(const Vect2<T>& right) {
	return { this->x + right.x, this->y + right.y, 1 };
}

template <typename T>
Vect2<T>& Vect2<T>::operator+=(const Vect2<T>& right) {
	this->x += right.x; this->y += right.y;
	return *this;
}

template <typename T>
Vect2<T> Vect2<T>::operator-(const Vect2<T>& right) {
	return { this->x - right.x, this->y - right.y, 1 };
}

template <typename T>
Vect2<T>& Vect2<T>::operator-=(const Vect2<T>& right) {
	this->x -= right.x; this->y -= right.y;
	return *this;
}

template <typename T>
Vect2<T> Vect2<T>::operator*(const Vect2<T>& right) {
	return { this->x * right.x, this->y * right.y, 1 };
}

template <typename T>
Vect2<T>& Vect2<T>::operator *= (const Vect2<T>& right) {
	this->x *= right.x; this->y *= right.y;
	return *this;
}

template <typename T>
Vect2<T> Vect2<T>::operator*(const T& right) {
	return { this->x * right, this->y * right, this->w };
}

template <typename T>
Vect2<T>& Vect2<T>::operator*=(const T& right) {
	this->x *= right; this->y *= right;
	return *this;
}

template <typename T>
Vect2<T> Vect2<T>::operator/(const T& right) {
	return { this->x / right, this->y / right, this->w };
}

//template <typename T>
//Vect2<T>& Vect2<T>::operator/=(const T& right) {
//	this->x /= right; this->y /= right;
//	return *this;
//}

template <typename T>
Vect2<T>& Vect2<T>::multiplyEach(const Vect2<T>& right) {
	this->x *= right.x; this->y *= right.y;
	return *this;
}

template <typename T>
Vect2<T> Vect2<T>::multiplyEach(const Vect2<T>& left, const Vect2<T>& right) {
	return { left.x * right.x, left.y * right.y, 1 };
}

template <typename T>
T Vect2<T>::dot(const Vect2<T>& right) {
	return this->x * right.x + this->y * right.y;
}

template <typename T>
T Vect2<T>::dot(const Vect2<T>& left, const Vect2<T>& right) {
	return left.x * right.x + left.y * right.y;
}

template <typename T>
T Vect2<T>::length() {
	return sqrtf(this->x * this->x + this->y * this->y);
}

template <typename T>
Vect2<T>& Vect2<T>::normalize() {
	float mag = length();
	this->x /= mag;
	this->y /= mag;
	return *this;
}

template <typename T>
Vect2<T> Vect2<T>::normalize(Vect2<T> v) {
	return v.normalize();
}

template <typename T>
Vect2<T> Vect2<T>::interpolate(const float& fraction, const Vect2<T>& left, const Vect2<T>& right) {
	return { interPolate(fraction, left.u, right.u), interPolate(fraction, left.v, right.v) };
}


//template <typename T>
//Vect3<T> Vect3<T>::operator-() {
//	return { -this->x, -this->y, -this->z, this->w };
//}

template <typename T>
Vect3<T> Vect3<T>::operator+(const Vect3<T>& right) {
	return { this->x + right.x, this->y + right.y, this->z + right.z, this->w };
}

template <typename T>
Vect3<T>& Vect3<T>::operator+=(const Vect3<T>& right) {
	this->x += right.x; this->y += right.y; this->z += right.z;
	return *this;
}

template <typename T>
Vect3<T> Vect3<T>::operator-(const Vect3<T>& right) {
	return { this->x - right.x, this->y - right.y, this->z - right.z, this->w };
}

//template <typename T>
//Vect3<T>& Vect3<T>::operator-=(const Vect3<T>& right) {
//	this->x -= right.x; this->y -= right.y; this->z -= right.z;
//	return *this;
//}

template <typename T>
Vect3<T> Vect3<T>::operator*(const Vect3<T>& right) {
	//i  j  k 
	//x  y  z
	//rx ry rz

	return { this->y * right.z - right.y * this->z,
			 this->z * right.x - right.y * this->x,
			 this->x * right.y - right.x * this->y,
			 this->w };
}

//template <typename T>
//Vect3<T> Vect3<T>::operator*(const T& right){
//return { this->x * right, this->y * right, this->z * right, this->w };
//};

template <typename T>
Vect3<T>& Vect3<T>::operator*=(const T& right) {
	this->x *= right; this->y *= right; this->z *= right;
	return *this;
}

template <typename T>
Vect3<T> Vect3<T>::operator/(const T& right) {
	return { this->x / right, this->y / right, this->z / right, this->w };
}

template <typename T>
Vect3<T>& Vect3<T>::operator/=(const T& right) {
	this->x /= right; this->y /= right; this->z /= right;
	return *this;
}

template <typename T>
Vect3<T>& Vect3<T>::multiplyEach(const Vect3<T>& right) {
	this->x *= right.x; this->y *= right.y; this->z *= right.z;
	return *this;
}

template <typename T>
Vect3<T> Vect3<T>::multiplyEach(const Vect3<T>& left, const Vect3<T>& right) {
	return { left.x * right.x, left.y * right.y, left.z * right.z, 1 };
}

template <typename T>
T Vect3<T>::dot(const Vect3<T>& right) {
	return this->x * right.x + this->y * right.y + this->z * right.z;
}

template <typename T>
T Vect3<T>::dot(const Vect3<T>& left, const Vect3<T>& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <typename T>
T Vect3<T>::length() {
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

template <typename T>
Vect3<T>& Vect3<T>::normalize() {
	float mag = length();
	this->x /= mag;
	this->y /= mag;
	this->z /= mag;
	return *this;
}

template <typename T>
Vect3<T> Vect3<T>::normalize(Vect3<T> v) {
	return v.normalize();
}

template <typename T>
Vect3<T> Vect3<T>::intersectPlane(Vect3<T>& planeP, Vect3<T>& planeN, Vect3<T>& lineStart, Vect3<T>& lineEnd, float& t) {
	planeN.normalize();
	float planeD = -planeN.dot(planeP);
	float ad = lineStart.dot(planeN);
	float bd = lineEnd.dot(planeN);
	t = (planeD + ad) / (ad - bd);

	Vect3 lineStartToEnd = lineEnd - lineStart;
	Vect3 lineToIntersect = lineStartToEnd * t;
	return lineStart + lineToIntersect;
}

Triangle Triangle::operator+(const Vec3& right) {
	Triangle tri(*this);
	tri.vertex[0].position += right;
	tri.vertex[1].position += right;
	tri.vertex[2].position += right;
	return tri;
}
Triangle& Triangle::operator+=(const Vec3& right) {
	this->vertex[0].position += right;
	this->vertex[1].position += right;
	this->vertex[2].position += right;
	return *this;
}
Triangle Triangle::operator*(const Vec3& right) {
	Triangle tri(*this);
	tri.vertex[0].position.multiplyEach(right);
	tri.vertex[1].position.multiplyEach(right);
	tri.vertex[2].position.multiplyEach(right);
	return tri;
}
Triangle& Triangle::operator*=(const Vec3& right) {
	this->vertex[0].position.multiplyEach(right);
	this->vertex[1].position.multiplyEach(right);
	this->vertex[2].position.multiplyEach(right);
	return *this;
}
Triangle Triangle::operator/(const float& right) {
	Triangle tri(*this);
	tri.vertex[0].position /= right;
	tri.vertex[1].position /= right;
	tri.vertex[2].position /= right;
	return tri;
}
Triangle& Triangle::operator/=(const float& right) {
	this->vertex[0].position /= right;
	this->vertex[1].position /= right;
	this->vertex[2].position /= right;
	return *this;
}
Vec3 Triangle::normal() {
	return ((vertex[1].position - vertex[0].position) * (vertex[2].position - vertex[0].position)).normalize();
}
Color Triangle::avgColor() {
	return  (vertex[0].color / 3 + vertex[1].color / 3 + vertex[2].color / 3);
}
Triangle& Triangle::normalize() {
	for (int i = 0; i < 3; i++)
		vertex[i].position /= vertex[i].position.w;
	return *this;
}
int Triangle::ClipAgainstPlane(Vec3 planeP, Vec3 planeN, Triangle& in, Triangle& out1, Triangle& out2) {
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
		dis[i] = d(in.vertex[i].position);
	for (int i = 0; i < 3; i++) {
		if (dis[i] >= 0) {
			insides[nInsidePointCount++] = &in.vertex[i].position;
			insideTextures[nInsideTextureCount++] = &in.vertex[i].textureCood;
		}
		else {
			outsides[nOutsidePointCount++] = &in.vertex[i].position;
			outsideTextures[nOutsideTextureCount++] = &in.vertex[i].textureCood;
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
			for (int i = 0; i < 3; i++) {
				out1.vertex[i].color = in.vertex[i].color;
				out1.vertex[i].intensity = in.vertex[i].intensity;
				out1.vertex[i].normal = in.vertex[i].normal;
			}

			out1.vertex[0].position = *insides[0];
			out1.vertex[0].textureCood = *insideTextures[0];

			//Two new points, at the location where
			//original sides of the triangle intersect the plane
			float t;
			out1.vertex[1].position = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[0], t);
			//out1.vertex[1].textureCood=Vec2::interpolate(t, *insideTextures[0], *outsideTextures[0]);
			out1.vertex[1].textureCood.u = interPolate(t, insideTextures[0]->u, outsideTextures[0]->u);
			out1.vertex[1].textureCood.v = interPolate(t, insideTextures[0]->v, outsideTextures[0]->v);

			out1.vertex[2].position = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[1], t);
			out1.vertex[2].textureCood.u = interPolate(t, insideTextures[0]->u, outsideTextures[1]->u);
			out1.vertex[2].textureCood.v = interPolate(t, insideTextures[0]->v, outsideTextures[1]->v);

			return 1;
		}
		break;
	case 2:
		if (nOutsidePointCount == 1) {
			//1 point lies outside
			//triangle becomes a quad

			//Set Output
			for (int i = 0; i < 3; i++) {
				out1.vertex[i].color = in.vertex[i].color;
				out1.vertex[i].intensity = in.vertex[i].intensity;
				out1.vertex[i].normal = in.vertex[i].normal;

				out2.vertex[i].color = in.vertex[i].color;
				out2.vertex[i].intensity = in.vertex[i].intensity;
				out2.vertex[i].normal = in.vertex[i].normal;
			}

			//First triangle consists of 2 inside points and
			//location where one side intersects the plane
			out1.vertex[0].position = *insides[0];
			out1.vertex[1].position = *insides[1];
			out1.vertex[0].textureCood = *insideTextures[0];
			out1.vertex[1].textureCood = *insideTextures[1];

			float t;
			out1.vertex[2].position = Vec3::intersectPlane(planeP, planeN, *insides[0], *outsides[0], t);
			out1.vertex[2].textureCood.u = interPolate(t, insideTextures[0]->u, outsideTextures[0]->u);
			out1.vertex[2].textureCood.v = interPolate(t, insideTextures[0]->v, outsideTextures[0]->v);

			//Second triangle consists of 1 inside point and
			//location where other side intersects the plane
			//and the newly created point;
			out2.vertex[0].position = *insides[1];
			out2.vertex[0].textureCood = *insideTextures[1];

			out2.vertex[1].position = out1.vertex[2].position;
			out2.vertex[1].textureCood = out1.vertex[2].textureCood;

			out2.vertex[2].position = Vec3::intersectPlane(planeP, planeN, *insides[1], *outsides[0], t);
			out2.vertex[2].textureCood.u = interPolate(t, insideTextures[1]->u, outsideTextures[0]->u);
			out2.vertex[2].textureCood.v = interPolate(t, insideTextures[1]->v, outsideTextures[0]->v);

			return 2;
		}
		break;
	}
	return 0;
}