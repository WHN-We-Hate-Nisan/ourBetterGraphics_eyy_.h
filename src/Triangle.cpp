#pragma once
#include"Essentials.h"
#include "Triangle.h"

int ClipAgainstPlane(Vec3 planeP, Vec3 planeN, Triangle& in, Triangle& out1, Triangle& out2) {
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

		DrawPixel(i, y, Color(r * a, g * a, b * a, 0xff));
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
