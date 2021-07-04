#pragma once
#include "Shape2D.h"

auto Shape2D::matrixify(std::vector<Vect2<float>> vect) {
	for (int i = 0; i < 3; i++)
		for (int k = 0; k < vect.size(); k++)
			tempMatrix[i][k] = (i == 0) ? vect[k].x : (i == 1) ? vect[k].y : 1;
	return tempMatrix;
}

auto Shape2D::vectorify(float matrix[3][max_Vertex]) {
	tempVector.clear();
	for (int k = 0; k < n; k++)
		tempVector.push_back(Vect3<float>(matrix[0][k], matrix[1][k], matrix[2][k]));
	return tempVector;
}

Shape2D::Shape2D(float rad, float angle, int side, float offX = getMidX(), float offY = getMidY()) :radius(rad), n(side) {
	angle *= pi / n;
	for (int i = 0; i < n; i++)
		vertSet.push_back({ radius * cos(2 * pi * i / n + angle) + offX,
							radius * sin(2 * pi * i / n + angle) + offY });
}

Shape2D::Shape2D() :n(3), radius(25) {
	for (int i = 0; i < n; i++)
		vertSet.push_back({ radius * cos(2 * pi * i / n + pi / n) + getMidX() / 4 ,
							radius * sin(2 * pi * i / n + pi / n) + getMidY() / 4 });
}

void Shape2D::DrawShape(unsigned color = 0) {
	//Draw Output
	bool colRand = !color;
	for (int i = 0; i < n; i++) {
		if (colRand) color = 0xffffff * (i + 1) / n;
		DrawBresLine(vertSet[i], vertSet[(i + 1) % n], color);
	}
}

void Shape2D::DrawShapeOrigin(unsigned color = 0) {
	//Draw Output
	bool colRand = !color;
	Vect2<float> offset = { float(getMidX()), float(getMidY()) };
	for (int i = 0; i < n; i++) {
		if (colRand) color = 0xffffff * (i + 1) / n;
		DrawBresLine(vertSet[i] + offset, vertSet[(i + 1) % n] + offset, color);
	}
}
//everything below needs doing - DankBros 12:30AM signing off

void Shape2D::matMul(float composite[3][3], float ans[3][max_Vertex]) {
	auto calculationMatrix = matrixify(vertSet);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < n; j++)
			ans[i][j] = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				ans[i][j] += composite[i][k] * calculationMatrix[k][j];
}

void Shape2D::translation(float newPoint[3][max_Vertex], Vect2<float> translationFactor) {
	float composite[3][3] = {
								{1,	 0,	translationFactor.x},
								{0,  1,	translationFactor.y},
								{0,	 0,					  1}
	};
	matMul(composite, newPoint);
	vertSet = vectorify(newPoint);
}

void Shape2D::rotation(float newPoint[3][max_Vertex], int angle) {
	float rad = 3.14 * angle / 180;
	float composite[3][3] = { {cos(rad),-sin(rad),0},{sin(rad),cos(rad),0},{0,0,1} };
	matMul(composite, newPoint);
	vertSet = vectorify(newPoint);
}

void Shape2D::rotate(float newPoint[3][max_Vertex], float rad) {
	float composite[3][3] = { {cos(rad),-sin(rad),0},{sin(rad),cos(rad),0},{0,0,1} };
	matMul(composite, newPoint);
	vertSet = vectorify(newPoint);
}

void Shape2D::scaling(float newPoint[3][max_Vertex], Vect2<float> scalingFactor) {
	float composite[3][3] = { {scalingFactor.x,0,0},{0,scalingFactor.y,0},{0,0,1} };
	matMul(composite, newPoint);
	vertSet = vectorify(newPoint);
}

void Shape2D::scale(float newPoint[3][max_Vertex], float sx, float sy) {
	float composite[3][3] = { {sx,0,0},{0,sy,0},{0,0,1} };
	matMul(composite, newPoint);
	vertSet = vectorify(newPoint);
}

void Shape2D::rotationPivot(float newPoint[3][max_Vertex], int angle, Vect2<int> pivot) {
	float rad = 3.14 * angle / 180;
	float composite[3][3] = { {cos(rad),-sin(rad),pivot.x * (1 - cos(rad)) + pivot.y * sin(rad)},
		{sin(rad),cos(rad),pivot.y * (1 - cos(rad)) - pivot.x * sin(rad)},{0,0,1} };
	matMul(composite, newPoint);
	vertSet = vectorify(newPoint);
}

void Shape2D::scalingFixed(float newPoint[3][max_Vertex], Vect2<int> fixedPoint, Vect2<float> scalingFactor) {
	float composite[3][3] = { {scalingFactor.x,0,fixedPoint.x * (1 - fixedPoint.x)},
		{0,scalingFactor.y,fixedPoint.y * (1 - scalingFactor.y)},{0,0,1} };
	matMul(composite, newPoint);
	vertSet = vectorify(newPoint);
}

void Shape2D::scalingWithAngle(float newPoint[3][max_Vertex], int angle, Vect2<float> scalingFactor) {
	float rad = 3.14 * angle / 180;
	rotate(newPoint, -rad);
	scale(newPoint, scalingFactor.x, scalingFactor.y);
	rotate(newPoint, rad);
}

//1.y=0 2.x=0 3.y=x 4.y=-x
void Shape2D::reflection(float newPoint[3][max_Vertex], int choice) {
	switch (choice)
	{
	case 1: {
		float composite[3][3] = { {1,0,0},{0,-1,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
		break;
	}
	case 2: {
		float composite[3][3] = { {-1,0,0},{0,1,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
		break;
	}
	case 3: {
		float composite[3][3] = { {0,1,0},{1,0,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
		break;
	}
	case 4: {
		float composite[3][3] = { {0,-1,0},{-1,0,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
		break;
	}
	}
}

void Shape2D::shearing(float newPoint[3][max_Vertex], int choice, Vect2<float> shearingParams) {
	//1.x-direction relative to x-axis(y=0) 2.y-direction relative to yaxis(x=0) 3.In both direction
	switch (choice) {
	case 1: {
		float composite[3][3] = { {1,shearingParams.x,0},{0,1,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
		break;
	}
	case 2: {
		float composite[3][3] = { {1,0,0},{shearingParams.y,1,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
		break;
	}
	case 3: {
		float composite[3][3] = { {1,shearingParams.x,0},{shearingParams.y,1,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
	}
	default:
		break;
	}
}

void Shape2D::shearigRef(float newPoint[3][max_Vertex], int choice, float shearingParameter, float referenceParameter)
{
	//1.x-direction relative to y=yref 2.y-direction relative to x=xref
	switch (choice) {
	case 1: {
		float composite[3][3] = { {1,shearingParameter,-shearingParameter * referenceParameter},{0,1,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
		break;
	}
	case 2: {
		float composite[3][3] = { {1,0,0},{shearingParameter,1,-shearingParameter * referenceParameter},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
		break;
	}
	default:
		break;
	}
}

void TestCase() {
	Shape2D pram[9];
	pram[0].DrawShapeOrigin(0xffffffff);
	//DrawBresLine( Vect2<int> { 500, 0 },  Vect2<int> { 500, 800 }, 0x90ee90);
	//DrawBresLine( Vect2<int> { 0, 400 },  Vect2<int> { 1000, 400 }, 0x90ee90);
	////float point[3][3] = { {x1,x2,x3},{y1,y2,y3},{1,1,1} };
	float newPoint[3][max_Vertex] = {};
	//DrawBresLine( Vect2<int> { x1 + 500, y1 + 400 },  Vect2<int> { x2 + 500, y2 + 400 }, 0x90ee99);
	//DrawBresLine( Vect2<int> { x2 + 500, y2 + 400 },  Vect2<int> { x3 + 500, y3 + 400 }, 0x90ee99);
	//DrawBresLine( Vect2<int> { x3 + 500 , y3 + 400 },  Vect2<int> { x1 + 500, y1 + 400 }, 0x90ee99);
	DrawBresLine(Vect2<int> {0, getMidY()}, Vect2<int>{getMidX() * 2, getMidY()}, 0x90ee99);
	DrawBresLine(Vect2<int> {getMidX(), 0}, Vect2<int>{getMidX(), getMidY() * 2}, 0x90ee99);
	////1.Translation with specified parameters
	pram[0].translation(newPoint, Vect2<float>{ 25, 30 });
	pram[0].DrawShapeOrigin(0xffadad);
	//2.Rotation with specified parameters
	pram[1].rotation(newPoint, 180);
	pram[1].DrawShapeOrigin(0xffd6a5);
	//3.Scaling with specified parameters
	pram[2].scaling(newPoint, Vect2<float>{ 2.6, 1.3 });
	pram[2].DrawShapeOrigin(0xfdffb6);
	//4.Rotation about a pivot point
	pram[3].rotationPivot(newPoint, 45, Vect2<int> { -100, 50 });
	pram[3].DrawShapeOrigin(0xcaffbf);
	//5.Scaling about a fixed point
	pram[4].scalingFixed(newPoint, Vect2<int> { 10, -10 }, Vect2<float>{ 2.6, 1.3 });
	pram[4].DrawShapeOrigin(0x9bf6ff);
	//6.Scaling with orthogonal axis at certain	angle from x - axis
	pram[5].scalingWithAngle(newPoint, 60, Vect2<float>{ 1.6, 2.1 });
	pram[5].DrawShapeOrigin(0x9bf6ff);
	//7.Reflection with specified axis
	pram[6].reflection(newPoint, 1);
	pram[6].DrawShapeOrigin(0xa0c4ff);
	//reflection(point, newPoint,2); DrawShape(newPoint);
	//reflection(point, newPoint,3); DrawShape(newPoint);
	//reflection(point, newPoint,4); DrawShape(newPoint);

//8.Shearing with specified parameter
	pram[7].shearing(newPoint, 3, Vect2<float>{ -1.2, 0.5 });
	pram[7].DrawShapeOrigin(0xbdb2ff);
	//shearing(point, newPoint, 2, Vect2{ -1.2, 0.5 }); DrawShape(newPoint);
	//shearing(point, newPoint, 1, Vect2{ -1.2, 0.5 }); DrawShape(newPoint);

//9.Shearing with specified reference line
	//shape[8].shearigRef(newPoint, 1, 2, 2); 
	//shape[8].DrawShapeOrigin(0xffc6ff);
}

