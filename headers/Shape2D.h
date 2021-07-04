#pragma once
#include"Essentials.h"

class Shape2D {
	typedef std::vector<Vect2<float>> VertSet;
	VertSet vertSet;
	int n;
	float radius;
	float tempMatrix[3][max_Vertex] = {0};
	VertSet tempVector;
	auto matrixify(std::vector<Vect2<float>> vect) {
		for (int i = 0; i < 3; i++)
			for (int k = 0; k < vect.size(); k++)
				tempMatrix[i][k] = (i == 0) ? vect[k].x : (i == 1) ? vect[k].y : 1;
		return tempMatrix;
	}
	auto vectorify(float matrix[3][max_Vertex]) {
		tempVector.clear();
		for (int k = 0; k < n; k++)
			tempVector.push_back(Vect3<float>(matrix[0][k], matrix[1][k], matrix[2][k]));
		return tempVector;
	}
public:
	Shape2D(float rad, float angle, int side, float offX=getMidX(), float offY= getMidY()):radius(rad),n(side) {
		angle *=  pi / n;
		for (int i = 0; i < n; i++)
			vertSet.push_back({ radius * cos(2 * pi * i / n + angle) + offX, 
								radius * sin(2 * pi * i / n + angle) + offY });
	}
	Shape2D():n(3), radius(100) {
		for (int i = 0; i < n; i++)
			vertSet.push_back({ radius * cos(2 * pi * i / n + pi / n) + getMidX(),
								radius * sin(2 * pi * i / n + pi / n) + getMidY()});
	}
	void DrawShape(unsigned color = 0) {
		//Draw Output
		bool colRand = !color;
		for (int i = 0; i < n; i++) {
			if (colRand) color = 0xffffff*(i+1)/n;
			DrawBresLine(vertSet[i], vertSet[(i + 1) % n], color);
		}
	}
	//everything below needs doing - DankBros 12:30AM signing off
	
	void matMul(float composite[3][3], float ans[3][max_Vertex]) {
		auto calculationMatrix = matrixify(vertSet);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < n; j++)
				ans[i][j] = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < n; j++)
				for (int k = 0; k < n; k++)
					ans[i][j] += composite[i][k] * calculationMatrix[k][j];
	}

	void translation(float newPoint[3][max_Vertex], Vect2<float> translationFactor) {
		float composite[3][3] = { 
									{1,	 0,	translationFactor.x},
									{0,  1,	translationFactor.y},
									{0,	 0,					  1} 
								};
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
	}
	void rotation(float newPoint[3][max_Vertex], int angle) {
		float rad = 3.14 * angle / 180;
		float composite[3][3] = { {cos(rad),-sin(rad),0},{sin(rad),cos(rad),0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
	}
	void rotate(float newPoint[3][max_Vertex], float rad) {
		float composite[3][3] = { {cos(rad),-sin(rad),0},{sin(rad),cos(rad),0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
	}
	void scaling(float newPoint[3][max_Vertex], Vect2<float> scalingFactor) {
		float composite[3][3] = { {scalingFactor.x,0,0},{0,scalingFactor.y,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
	}
	void scale(float newPoint[3][max_Vertex], float sx, float sy) {
		float composite[3][3] = { {sx,0,0},{0,sy,0},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
	}
	void rotationPivot(float newPoint[3][max_Vertex], int angle, Vect2<int> pivot) {
		float rad = 3.14 * angle / 180;
		float composite[3][3] = { {cos(rad),-sin(rad),pivot.x * (1 - cos(rad)) + pivot.y * sin(rad)},
			{sin(rad),cos(rad),pivot.y * (1 - cos(rad)) - pivot.x * sin(rad)},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
	}
	void scalingFixed(float newPoint[3][max_Vertex], Vect2<int> fixedPoint, Vect2<float> scalingFactor) {
		float composite[3][3] = { {scalingFactor.x,0,fixedPoint.x * (1 - fixedPoint.x)},
			{0,scalingFactor.y,fixedPoint.y * (1 - scalingFactor.y)},{0,0,1} };
		matMul(composite, newPoint);
		vertSet = vectorify(newPoint);
	}
	void scalingWithAngle(float newPoint[3][max_Vertex], int angle, Vect2<float> scalingFactor) {
		float rad = 3.14 * angle / 180;
		rotate(newPoint, -rad);
		scale(newPoint, scalingFactor.x, scalingFactor.y);
		rotate(newPoint, rad);
	}
	//1.y=0 2.x=0 3.y=x 4.y=-x
	void reflection(float newPoint[3][max_Vertex], int choice) {
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
	void shearing(float newPoint[3][max_Vertex], int choice, Vect2<float> shearingParams) {
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
	void shearigRef(float newPoint[3][max_Vertex], int choice, float shearingParameter, float referenceParameter)
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
};

void Transformations() {
	int x1 = 80, x2 = 60, x3 = 55, y1 = 90, y2 = 105, y3 = 90;
	DrawBresLine( Vect2<int> { 500, 0 },  Vect2<int> { 500, 800 }, 0x90ee90);
	DrawBresLine( Vect2<int> { 0, 400 },  Vect2<int> { 1000, 400 }, 0x90ee90);
	float point[3][3] = { {x1,x2,x3},{y1,y2,y3},{1,1,1} };
	float newPoint[3][3] = {};
	DrawBresLine( Vect2<int> { x1 + 500, y1 + 400 },  Vect2<int> { x2 + 500, y2 + 400 }, 0x90ee99);
	DrawBresLine( Vect2<int> { x2 + 500, y2 + 400 },  Vect2<int> { x3 + 500, y3 + 400 }, 0x90ee99);
	DrawBresLine( Vect2<int> { x3 + 500 , y3 + 400 },  Vect2<int> { x1 + 500, y1 + 400 }, 0x90ee99);

	//1.Translation with specified parameters
	translation(newPoint, Vect2{ 25,30 }); DrawShape(newPoint, 0xffadad);
	//2.Rotation with specified parameters
	rotation(newPoint, 150); DrawShape(newPoint, 0xffd6a5);
	//3.Scaling with specified parameters
	scaling(newPoint, Vect2{ 2.6, 1.3 }); DrawShape(newPoint, 0xfdffb6);
	//4.Rotation about a pivot point
	rotationPivot( newPoint, 45,  Vect2<int> { -100,50 }); DrawShape(newPoint, 0xcaffbf);
	//5.Scaling about a fixed point
	scalingFixed( newPoint,  Vect2<int> { 35, -80 }, Vect2{ 2.6, 1.3 }); DrawShape(newPoint, 0x9bf6ff);
	//6.Scaling with orthogonal axis at certain	angle from x - axis
	scalingWithAngle( newPoint, 60, Vect2{ 1.6, 2.1 }); DrawShape(newPoint, 0x9bf6ff);
	//7.Reflection with specified axis
	reflection(newPoint, 1); DrawShape(newPoint, 0xa0c4ff);
	//reflection(point, newPoint,2); DrawShape(newPoint);
	//reflection(point, newPoint,3); DrawShape(newPoint);
	//reflection(point, newPoint,4); DrawShape(newPoint);

//8.Shearing with specified parameter
	shearing(point, newPoint, 3, Vect2{ -1.2, 0.5 }); DrawShape(newPoint, 0xbdb2ff);
	//shearing(point, newPoint, 2, Vect2{ -1.2, 0.5 }); DrawShape(newPoint);
	//shearing(point, newPoint, 1, Vect2{ -1.2, 0.5 }); DrawShape(newPoint);

//9.Shearing with specified reference line
	shearigRef(point, newPoint, 1, 2, 2); DrawShape(newPoint, 0xffc6ff);
//}
