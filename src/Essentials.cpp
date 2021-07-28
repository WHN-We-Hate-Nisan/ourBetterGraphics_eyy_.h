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
void Swap(T &xp, T &yp){
	T temp = xp;
	xp = yp;
	yp = temp;
}
unsigned int interPolate(const float& input1, const float& input2, const float& position, const unsigned int& val1, const unsigned int& val2) {
	if (input1 == input2) return val1;
	else return val1 +  (position - input1) / (input2 - input1) * (val2 - val1);
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
			if (arr[j].y > arr[j + 1].y)
				Swap(arr[j], arr[j + 1]);
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
	/*if (v1.x < 0 || v1.x >= globalBuffer.width || v1.y < 0 || v1.y >= globalBuffer.height) {
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
	}*/
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
	if (x2 < x1) Swap(x1, x2);
	for (int i = x1; i <= x2; i++)
		DrawPixel(i + off.x, y + off.y, color);
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
	DrawBresLine(t.vertex[0], t.vertex[1], color);
	DrawBresLine(t.vertex[1], t.vertex[2], color);
	DrawBresLine(t.vertex[2], t.vertex[0], color);
}
void ColorTriangle(Triangle& tri, Color color, Vec3 off) {
	float dx1, dx2, dx3;
	Vec3 array[] = { tri.vertex[0], tri.vertex[1], tri.vertex[2] };
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
void TextureTriangle(Triangle& tri, Texture* texture) {
	//Sort  Vertices by y value
	Vec3 array[] = { tri.vertex[0], tri.vertex[1], tri.vertex[2] };
	Vec2 textureArray[] = { tri.texCood[0], tri.texCood[1], tri.texCood[2] };
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
			DrawHorizTexture(ax, bx, i, texU, texV, texW, texStartU, texEndU, texStartV, texEndV,texStartW, texEndW, texture);
		}
	}
}

int getMidX() {
	return globalBuffer.width / 2;
}
int getMidY() {
	return globalBuffer.height / 2;
}
