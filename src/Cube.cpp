#pragma once
#include"Essentials.h"
#include "..\headers\Cube3D.h"

auto Cube3D::matrixify(VertSet vect)
{
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < this->n; k++)
			tempMatrix[i][k] = (i == 0) ? vect[k].x : (i == 1) ? vect[k].y : (i == 2) ? vect[k].z : 1;
	return tempMatrix;
}
auto Cube3D::matrixify(Vect3<float> vect){
	for (int i = 0; i < 4; i++)
		tempMat[i] = (i == 0) ? vect.x : (i == 1) ? vect.y : (i == 2) ? vect.z : 1;
	return tempMat;
}
auto Cube3D::vectorify(float matrix[4], int index)
{
	vertSet[index] = Vect3<float>(matrix[0], matrix[1], matrix[2]);
}
Cube3D::Cube3D(int cubeWidth)
{
	Vect3<float>
		A(0, 0, 0),
		B(0, 0, cubeWidth),
		C(0, cubeWidth, cubeWidth),
		D(0, cubeWidth, 0),
		E(cubeWidth, cubeWidth, 0),
		F(cubeWidth, 0, 0),
		G(cubeWidth, 0, cubeWidth),
		H(cubeWidth, cubeWidth, cubeWidth),
		x1(-getMidX() + 1, cubeWidth / 2, cubeWidth / 2),
		x2(getMidX() - 1, cubeWidth / 2, cubeWidth / 2),
		y1(cubeWidth / 2, -getMidY() + 1, cubeWidth / 2),
		y2(cubeWidth / 2, getMidY() - 1, cubeWidth / 2),
		z1(cubeWidth / 2, cubeWidth / 2, -300),
		z0(cubeWidth / 2, cubeWidth / 2, 0),
		z2(cubeWidth / 2, cubeWidth / 2, 300);

	std::vector <Vect3<float>> arr = { B,C,H,G,A,D,E,F,x1,x2,y1,y2,z1,z0,z2 };
	this->vertSet = arr;
	this->n = vertSet.size();
}
Cube3D::Cube3D()
{
	int cubeWidth = 100;
	Vect3<float>
		A(0, 0, 0),
		B(0, 0, cubeWidth),
		C(0, cubeWidth, cubeWidth),
		D(0, cubeWidth, 0),
		E(cubeWidth, cubeWidth, 0),
		F(cubeWidth, 0, 0),
		G(cubeWidth, 0, cubeWidth),
		H(cubeWidth, cubeWidth, cubeWidth),
		x1(-getMidX() + 1, cubeWidth / 2, cubeWidth / 2),
		x2(getMidX() - 1, cubeWidth / 2, cubeWidth / 2),
		y1(cubeWidth / 2, -getMidY() + 1, cubeWidth / 2),
		y2(cubeWidth / 2, getMidY() - 1, cubeWidth / 2),
		z1(cubeWidth / 2, cubeWidth / 2, -300),
		z0(cubeWidth / 2, cubeWidth / 2, 0),
		z2(cubeWidth / 2, cubeWidth / 2, 300);

	std::vector <Vect3<float>> arr = { B,C,H,G,A,D,E,F,x1,x2,y1,y2,z1,z0,z2 };
	this->vertSet = arr;
	this->n = vertSet.size();
}
Cube3D::Cube3D(VertSet vertSet) {
	this->vertSet = vertSet;
	this->n = vertSet.size();
}
void Cube3D::transformShape(float matA[][4])
{
	float result[4] = { 0 };
	for (int index = 0; index < n; index++) {
		transformPoint(matA, result, index);
		vectorify(result, index);
	}
}
void Cube3D::transformPoint(float matA[4][4], float result[4], int index)
{
	float* matB = matrixify(vertSet[index]);
	memset(result, 0, sizeof(float) * 4);
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < 4; k++)
			result[i] += matA[i][k] * matB[k];
}
void Cube3D::draw() {
	int X = getMidX(), Y = getMidY();
	for (int i = 1; i < vertSet.size(); i++)
		DrawDDALine(vertSet[i - 1].x + X, vertSet[i - 1].y + Y, vertSet[i].x + X, vertSet[i].y + Y);
	DrawDDALine(vertSet[vertSet.size() - 1].x + X, vertSet[vertSet.size() - 1].y + Y, vertSet[0].x + X, vertSet[0].y + Y);
}
void Cube3D::translate(Vect3<float> e)
{
	float tranMatrix[4][4] = {
								{1,  0,   0,   e.x },
								{0,  1,   0,   e.y },
								{0,  0,   1,   e.z },
								{0,  0,   0,   1   }
	};
	transformShape(tranMatrix);
}
void Cube3D::scale(int s)
{
	float tranMatrix[4][4] = { {s,  0,   0,   0 },
								{0,  s,   0,   0 },
								{0,  0,   s,   0 },
								{0,  0,   0,   1 } };
	transformShape(tranMatrix);
}
void Cube3D::rotateZ(float theeta) {
	theeta *= pi / 180;
	float tranMatrix[4][4] = {
								cos(theeta),    -sin(theeta),   0,  0,
								sin(theeta),    cos(theeta),    0,  0,
								0,              0,              1,  0,
								0,              0,              0,  1
	};
	transformShape(tranMatrix);
}
void Cube3D::rotateX(float theeta) {
	theeta *= pi / 180;
	float tranMatrix[4][4] = {
								1,              0,              0,              0,
								0,              cos(theeta),    -sin(theeta),   0,
								0,              sin(theeta),    cos(theeta),    0,
								0,              0,              0,				1
	};
	transformShape(tranMatrix);
}
void Cube3D::rotateY(float theeta)
{
	theeta *= pi / 180;
	float tranMatrix[4][4] = {
								  cos(theeta),       0,       +sin(theeta),       0,
								  0,                 1,       0,                 0,
								  -sin(theeta),      0,       cos(theeta),       0,
								  0,                 0,       0,                 1
	};
	transformShape(tranMatrix);
}
void Cube3D::viewOld(Vect3<float> e, Vect3<float> v)
{
	translate(-e);
	// u,v,n axes

	Vect3<float> n = { 0,0,1 }; //find from a-e vector
	//Normalize v
	Vect3<float> u = { 1,0,0 }; //Find from VxN
	// here same as x,y,n
	float tranMatrix[4][4] = {
									{u.x,  u.y,   u.z,   0 },
									{v.x,  v.y,   v.z,   0 },
									{n.x,  n.y,   n.z,   0 },
									{0,	   0,     0,     1 }
	};
	transformShape(tranMatrix);
}
void Cube3D::view(Vect3<float> e, Vect3<float> v)
{
	// u,v,n axes		
	Vect3<float> n = { 0,0,1 }; //find from a-e vector
	Vect3<float> u = { 1,0,0 }; //Find from VxN

	float result[4] = { 0 };
	for (int index = 0; index < this->n; index++) {
		n = this->vertSet[index] - e;
		n.normalize();
		//calculate u and v
		float tranMatrix[4][4] = {
								{1,  0,   0,   e.x },
								{0,  1,   0,   e.y },
								{0,  0,   1,   e.z },
								{0,  0,   0,   1   }
		};
		float rotMatrix[4][4] = {
									{u.x,  u.y,   u.z,   0 },
									{v.x,  v.y,   v.z,   0 },
									{n.x,  n.y,   n.z,   0 },
									{0,	   0,     0,     1 }
		};
		transformPoint(tranMatrix, result, index);
		transformPoint(rotMatrix, result, index);
		vectorify(result, index);
	}
}
void Cube3D::orthographic_projection(bool x, bool y, bool z)
{
	float tranMatrix[4][4] = {
								!x,     0,          0,      0,
								0,      !y,         0,      0,
								0,      0,          !z,     0,
								0,      0,          0,      1
	};
	transformShape(tranMatrix);
}
void Cube3D::oblique_projection(float alpha, float theeta)
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
	transformShape(tranMatrix);
}
void Cube3D::perspective_projection(float xprp, float yprp, float zprp, float zvp)
{
	float depths[max_Vertex];
	for (int index = 0; index < n; index++)
		depths[index] = this->vertSet[index].z;
	float dp = zprp - zvp;
	if (!dp) {
		consoleLog("DP is zero\n");
		dp = 10;
	}
	if (!zprp) {
		consoleLog("Zprp is zero\n"); zprp = 1;
	}
	float tranMatrix[4][4] = {
								  1,         0,       xprp / dp,      -xprp * zvp / dp,
								  0,         1,       yprp / dp,      -yprp * zvp / dp,
								  0,         0,       -zvp / dp,    zvp * (zprp / dp),
								  0,         0,       -1 / dp,      zprp / dp
	};
	float result[4] = { 0 };
	for (int index = 0; index < n; index++) {
		transformPoint(tranMatrix, result, index);
		result[0] /= result[3];	result[1] /= result[3];
		result[2] /= result[3];	result[3] = 1;
		vectorify(result, index);
	}
	for (int index = 0; index < n; index++)
		this->vertSet[index].z = depths[index];
}
void Cube3D::colorTriangle(Vect3<float> A, Vect3<float> B, Vect3<float> C, unsigned int color, Vect3<float> off) {
	float dx1, dx2, dx3;
	Vect3<float> array[] = { A, B, C};
	SortByY(array);
	A = array[0];
	B = array[1];
	C = array[2];
	Vect3<float> Source, End;
	if (B.y - A.y > 0) dx1 = (B.x - A.x) / (B.y - A.y); else dx1 = 0;
	if (C.y - A.y > 0) dx2 = (C.x - A.x) / (C.y - A.y); else dx2 = 0;
	if (C.y - B.y > 0) dx3 = (C.x - B.x) / (C.y - B.y); else dx3 = 0;

	Source = End = A;
	float depthh=Source.z;
	if (dx1 > dx2) {
		for (; Source.y <= B.y; Source.y++, End.y++, Source.x += dx2, End.x += dx1)
		{		//DrawBresLine(Source, End, color, off);
			depthh = interPolateDepth(A.y, B.y, Source.y, A.z, B.z);
			DrawHorizLine(Source.x, End.x, Source.y, color, depthh, off);
		}
		End = B;
		for (; Source.y <= C.y; Source.y++, End.y++, Source.x += dx2, End.x += dx3)
		{	//DrawBresLine(Source, End, color, off);
			depthh = interPolateDepth(B.y, C.y, Source.y, B.z, C.z);
			DrawHorizLine(Source.x, End.x, Source.y, color, depthh, off);
		}
	}
	else {
		for (; Source.y <= B.y; Source.y++, End.y++, Source.x += dx1, End.x += dx2)
		{	//DrawBresLine(Source, End, color, off);
			depthh = interPolateDepth(A.y, B.y, Source.y, A.z, B.z);
			DrawHorizLine(Source.x, End.x, Source.y, color, depthh, off);
		}
		Source = B;
		for (; Source.y <= C.y; Source.y++, End.y++, Source.x += dx3, End.x += dx2)
		{	//DrawBresLine(Source, End, color, off);
			depthh = interPolateDepth(B.y, C.y, Source.y, B.z, C.z);
			DrawHorizLine(Source.x, End.x, Source.y, color, depthh, off);
		}
	}
}
void Cube3D::colorFace(Vect3<float> A, Vect3<float> B, Vect3<float> C, Vect3<float> D, unsigned int color, Vect3<float> off) {		
	colorTriangle(A, B, C, color, off);
	colorTriangle(C, D, A, color, off);
}
void Cube3D::drawCube(bool colored) {
	Vect3<float> off(getMidX(), getMidY(), 0);
	//B,C,H,G,A,D,E,F
	//0,1,2,3,4,5,6,7

	unsigned int front = 0xcaffbf, middle = 0x9bf6ff, back = 0xffffff;
	//Back
	DrawBresLine(vertSet[0], vertSet[3], back, off); //BG     A D
	DrawBresLine(vertSet[3], vertSet[2], back, off); //GH     D C
	DrawBresLine(vertSet[2], vertSet[1], back, off); //HC     C B
	DrawBresLine(vertSet[1], vertSet[0], back, off); //CB     B A

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


	//Axes
	DrawBresLine(vertSet[8], vertSet[9], 0xff0000, off); //x
	DrawBresLine(vertSet[10], vertSet[11], 0xff0000, off); //y
	DrawBresLine(vertSet[12], vertSet[13], 0xff0000, off); //z
	DrawBresLine(vertSet[14], vertSet[13], 0xff0000, off); //z

	//B,C,H,G,A,D,E,F
	//0,1,2,3,4,5,6,7
	if (colored) {
		colorFace(vertSet[0], vertSet[3], vertSet[2], vertSet[1], 0xffadad, off); //BGCH
		colorFace(vertSet[4], vertSet[0], vertSet[1], vertSet[5], 0xffd6a5, off); //ABCD
		colorFace(vertSet[7], vertSet[3], vertSet[2], vertSet[6], 0xfdffb6, off); //FGHE
		colorFace(vertSet[5], vertSet[1], vertSet[2], vertSet[6], 0xcaffbf, off); //CHED
		colorFace(vertSet[4], vertSet[0], vertSet[3], vertSet[7], 0x9bf6ff, off); //ABGF
		colorFace(vertSet[4], vertSet[7], vertSet[6], vertSet[5], 0xbdb2ff, off); //AFED
	}
}
void Cube3D::drawCubeOrigin(float x, float y, float z,
	unsigned int front, unsigned int middle, unsigned int back, bool colored) {
	Vect3<float> off(x, y, z);
	//B,C,H,G,A,D,E,F axes: x1,x2,y1,y2,z1,z0,z2
	//0,1,2,3,4,5,6,7       8,9,10,11,12,13,14

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

	//Axes
	DrawBresLine(vertSet[8], vertSet[9], 0xff0000, off); //x
	DrawBresLine(vertSet[10], vertSet[11], 0xff0000, off); //y
	DrawBresLine(vertSet[12], vertSet[13], 0xff0000, off); //z
	DrawBresLine(vertSet[14], vertSet[13], 0xff0000, off); //z
}