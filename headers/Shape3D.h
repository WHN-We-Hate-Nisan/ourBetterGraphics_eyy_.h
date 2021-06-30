#pragma once
#include"Essentials.h"

class Shape3D {
public:
	std::vector <Vect3<float>> vertSet;
	std::vector<Vect3<float>> tempVector;
	int n;
	float tempMatrix[4][max_Vertex] = { 0 };

	auto matrixify(std::vector<Vect3<float>> vect) {
		for (int i = 0; i < 4; i++)
			for (int k = 0; k < vect.size(); k++)
				tempMatrix[i][k] = (i == 0) ? vect[k].x : (i == 1) ? vect[k].y : (i == 2) ? vect[k].z : 1;
		return tempMatrix;
	}
	void matrixify(std::vector<Vect3<float>> vect, float resMat[4][max_Vertex]) {
		for (int i = 0; i < 4; i++)
			for (int k = 0; k < vect.size(); k++)
				resMat[i][k] = (i == 0) ? vect[k].x : (i == 1) ? vect[k].y : (i == 2) ? vect[k].z : 1;
	}
	auto vectorify(float matrix[][max_Vertex]) {
		tempVector.clear();
		for (int k = 0; k < n; k++)
			tempVector.push_back(Vect3<float>(matrix[0][k], matrix[1][k], matrix[2][k]));
		return tempVector;
	}
public:
	Shape3D() {
		int cubeWidth = 100;
		Vect3<float>
			A(0, 0, 0),
			B(0, 0, cubeWidth),
			C(0, cubeWidth, cubeWidth),
			D(0, cubeWidth, 0),
			E(cubeWidth, cubeWidth, 0),
			F(cubeWidth, 0, 0),
			G(cubeWidth, 0, cubeWidth),
			H(cubeWidth, cubeWidth, cubeWidth);
		std::vector <Vect3<float>> arr = { B,C,H,G,A,D,E,F };
		this->vertSet = arr;
		this->n = vertSet.size();
	}
	Shape3D(std::vector <Vect3<float>> vertSet) {
		this->vertSet = vertSet;
		this->n = vertSet.size();
	}
	void transformShape(float matA[][4], float matB[][max_Vertex], int n)
	{
		float result[4][max_Vertex] = { 0 };
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < n; j++)
				for (int k = 0; k < 4; k++)
					result[i][j] += matA[i][k] * matB[k][j];
		this->vertSet = vectorify(result);
	}
	void draw() {
		int X = getMidX(), Y = getMidY();
		for (int i = 1; i < vertSet.size(); i++)
			DrawDDALine(vertSet[i - 1].x + X, vertSet[i - 1].y + Y, vertSet[i].x + X, vertSet[i].y + Y);
		DrawDDALine(vertSet[vertSet.size() - 1].x + X, vertSet[vertSet.size() - 1].y + Y, vertSet[0].x + X, vertSet[0].y + Y);
	}
	void translate(int x, int y, int z)
	{
		float tranMatrix[4][4] = {
									{1,  0,   0,   x },
									{0,  1,   0,   y },
									{0,  0,   1,   z },
									{0,  0,   0,   1 }
		};
		transformShape(tranMatrix, matrixify(vertSet), this->n);
	}
	void scale(int s)
	{
		float tranMatrix[4][4] = { {s,  0,   0,   0 },
									{0,  s,   0,   0 },
									{0,  0,   s,   0 },
									{0,  0,   0,   1 } };
		transformShape(tranMatrix, matrixify(vertSet), this->n);
	}
	void rotateZ(float theeta) {
		theeta *= pi / 180;
		float tranMatrix[4][4] = {
									cos(theeta),    -sin(theeta),   0,  0,
									sin(theeta),    cos(theeta),    0,  0,
									0,              0,              1,  0,
									0,              0,              0,  1
		};
		transformShape(tranMatrix, matrixify(vertSet), this->n);
	}
	void rotateX(float theeta) {
		theeta *= pi / 180;
		float tranMatrix[4][4] = {
									1,              0,              0,              0,
									0,              cos(theeta),    -sin(theeta),   0,
									0,              sin(theeta),    cos(theeta),    0,
									0,              0,              0,				1
		};
		transformShape(tranMatrix, matrixify(vertSet), this->n);
	}
	void rotateY(float theeta)
	{
		theeta *= pi / 180;
		float tranMatrix[4][4] = {
									  cos(theeta),       0,       +sin(theeta),       0,
									  0,                 1,       0,                 0,
									  -sin(theeta),      0,       cos(theeta),       0,
									  0,                 0,       0,                 1
		};
		transformShape(tranMatrix, matrixify(vertSet), this->n);
	}
	void orthographic_projection(bool x, bool y, bool z)
	{
		float tranMatrix[4][4] = {
									!x,     0,          0,      0,
									0,      !y,         0,      0,
									0,      0,          !z,     0,
									0,      0,          0,      1
		};
		transformShape(tranMatrix, matrixify(vertSet), this->n);
	}
	void oblique_projection(float alpha, float theeta)
	{
		if (alpha == 0 || alpha == 90 || alpha == 180 || alpha == 270 || alpha == 360) return;
		theeta *= pi / 180;
		alpha *= pi / 180;
		float l1 = 1 / tan(alpha);
		float tranMatrix[4][4] = {
									{1, 0, l1 * cos(theeta), 0},
									{0, 1, l1 * sin(theeta), 0},
									{0, 0, 0,				 0},
									{0, 0, 0,				 1}
		};
		transformShape(tranMatrix, matrixify(vertSet), this->n);
	}
	void perspective_projection(float zprp, float zvp)
	{
		float dp = zprp - zvp;
		if (!dp) {
			consoleLog("DP is zero");
			return;
		}
		float tranMatrix[4][4] = {
									  1,         0,       0,            0,
									  0,         1,       0,            0,
									  0,         0,       -zvp / dp,    zvp * (zprp / dp),
									  0,         0,       -1 / dp,      zprp / dp
		};
		float matB[4][max_Vertex] = { 0 };
		matrixify(vertSet, matB);
		float result[4][max_Vertex] = { 0 };
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < n; j++)
				for (int k = 0; k < 4; k++)
					result[i][j] += tranMatrix[i][k] * matB[k][j];
		for (int i = 0; i < this->n; i++)
		{
			result[0][i] /= result[3][i];
			result[1][i] /= result[3][i];
			result[2][i] /= result[3][i];
			result[3][i] = 1;
		}
		this->vertSet = vectorify(result);
	}
	void drawCube() {
		Vect3<float> off(getMidX(), getMidY(), 0);
		//B,C,H,G,A,D,E,F
		//0,1,2,3,4,5,6,7

		unsigned int front = 0xcaffbf, middle = 0x9bf6ff, back = 0xffffff;
		//Back
		DrawBresLine(vertSet[0], vertSet[3], back, off); //BG
		DrawBresLine(vertSet[3], vertSet[2], back, off); //GH
		DrawBresLine(vertSet[2], vertSet[1], back, off); //HC
		DrawBresLine(vertSet[1], vertSet[0], back, off); //CB

		//Middle
		DrawBresLine(vertSet[4], vertSet[0], middle, off); //AB
		DrawBresLine(vertSet[3], vertSet[7], middle, off); //FG
		DrawBresLine(vertSet[5], vertSet[1], middle, off); //DC
		DrawBresLine(vertSet[2], vertSet[6], middle, off); //EH

		//Front
		DrawBresLine(vertSet[4], vertSet[7], front, off); //AF
		DrawBresLine(vertSet[7], vertSet[6], front, off); //FE
		DrawBresLine(vertSet[6], vertSet[5], front, off); //ED
		DrawBresLine(vertSet[5], vertSet[4], front, off); //DA
	}
	void drawCubeOrigin() {
		//B,C,H,G,A,D,E,F
		//0,1,2,3,4,5,6,7

		unsigned int front = 0xcaffbf, middle = 0x9bf6ff, back = 0xffffff;
		//Back
		DrawBresLine(vertSet[0], vertSet[3], back); //BG
		DrawBresLine(vertSet[3], vertSet[2], back); //GH
		DrawBresLine(vertSet[2], vertSet[1], back); //HC
		DrawBresLine(vertSet[1], vertSet[0], back); //CB

		//Middle
		DrawBresLine(vertSet[4], vertSet[0], middle); //AB
		DrawBresLine(vertSet[3], vertSet[7], middle); //FG
		DrawBresLine(vertSet[5], vertSet[1], middle); //DC
		DrawBresLine(vertSet[2], vertSet[6], middle); //EH

		//Front
		DrawBresLine(vertSet[4], vertSet[7], front); //AF
		DrawBresLine(vertSet[7], vertSet[6], front); //FE
		DrawBresLine(vertSet[6], vertSet[5], front); //ED
		DrawBresLine(vertSet[5], vertSet[4], front); //DA
	}
};

