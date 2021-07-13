#pragma once
#include"Essentials.h"

class Shape2D {
	typedef std::vector<Vect2<float>> VertSet;
	VertSet vertSet;
	int n;
	float radius;
	float tempMatrix[3][max_Vertex] = {0};
	VertSet tempVector;
	auto matrixify(std::vector<Vect2<float>> vect);
	auto vectorify(float[3][max_Vertex]);
public:
	Shape2D(float, float, int, float, float);
	Shape2D();
	void DrawShape(unsigned);
	void DrawShapeOrigin(unsigned);

	//everything below needs doing - DankBros 12:30AM signing off
	void matMul(float[3][3], float[3][max_Vertex]);
	void translation(float[3][max_Vertex], Vect2<float>);
	void rotation(float[3][max_Vertex], int);
	void rotate(float[3][max_Vertex], float);
	void scaling(float[3][max_Vertex], Vect2<float>);
	void scale(float[3][max_Vertex], float, float);
	void rotationPivot(float[3][max_Vertex], int, Vect2<int>);
	void scalingFixed(float[3][max_Vertex], Vect2<int>, Vect2<float>);
	void scalingWithAngle(float[3][max_Vertex], int, Vect2<float>);
	//1.y=0 2.x=0 3.y=x 4.y=-x
	void reflection(float[3][max_Vertex], int);
	void shearing(float[3][max_Vertex], int, Vect2<float>);
	void shearigRef(float[3][max_Vertex], int, float, float);
};

void TestCase();