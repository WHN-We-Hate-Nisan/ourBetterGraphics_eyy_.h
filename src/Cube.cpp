#pragma once
#include"Essentials.h"
#include "Cube3D.h"

auto Cube3D::matrixify(VertSet vect)
{
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < this->n; k++)
			tempMatrix[i][k] = (i == 0) ? vect[k].x : (i == 1) ? vect[k].y : (i == 2) ? vect[k].z : 1;
	return tempMatrix;
}
auto Cube3D::matrixify(Vec3 vect){
	for (int i = 0; i < 4; i++)
		tempMat[i] = (i == 0) ? vect.x : (i == 1) ? vect.y : (i == 2) ? vect.z : 1;
	return tempMat;
}
auto Cube3D::vectorify(float matrix[4], int index)
{
	vertSet[index] = { matrix[0], matrix[1], matrix[2] };
}
Cube3D::Cube3D(int cubeWidth)
{
	Vec3
		A{ 0, 0, 0 },
		B{0, 0, cubeWidth},
		C{0, cubeWidth, cubeWidth},
		D{ 0, cubeWidth, 0 },
		E{ cubeWidth, cubeWidth, 0 },
		F{ cubeWidth, 0, 0 },
		G{ cubeWidth, 0, cubeWidth },
		H{ cubeWidth, cubeWidth, cubeWidth },
		x1{ -getMidX() + 1, cubeWidth / 2, cubeWidth / 2 },
		x2{ getMidX() - 1, cubeWidth / 2, cubeWidth / 2 },
		y1{ cubeWidth / 2, -getMidY() + 1, cubeWidth / 2 },
		y2{ cubeWidth / 2, getMidY() - 1, cubeWidth / 2 },
		z1{ cubeWidth / 2, cubeWidth / 2, -300 },
		z0{ cubeWidth / 2, cubeWidth / 2, 0 },
		z2{ cubeWidth / 2, cubeWidth / 2, 300 };

	std::vector <Vec3> arr = { B,C,H,G,A,D,E,F,x1,x2,y1,y2,z1,z0,z2 };
	this->vertSet = arr;
	this->n = vertSet.size();
}
Cube3D::Cube3D()
{
	int cubeWidth = 100;
	Vec3
		A{ 0, 0, 0 },
		B{ 0, 0, cubeWidth },
		C{ 0, cubeWidth, cubeWidth },
		D{ 0, cubeWidth, 0 },
		E{ cubeWidth, cubeWidth, 0 },
		F{ cubeWidth, 0, 0 },
		G{ cubeWidth, 0, cubeWidth },
		H{ cubeWidth, cubeWidth, cubeWidth },
		x1{ -getMidX() + 1, cubeWidth / 2, cubeWidth / 2 },
		x2{ getMidX() - 1, cubeWidth / 2, cubeWidth / 2 },
		y1{ cubeWidth / 2, -getMidY() + 1, cubeWidth / 2 },
		y2{ cubeWidth / 2, getMidY() - 1, cubeWidth / 2 },
		z1{ cubeWidth / 2, cubeWidth / 2, -300 },
		z0{ cubeWidth / 2, cubeWidth / 2, 0 },
		z2{ cubeWidth / 2, cubeWidth / 2, 300 };

	std::vector <Vec3> arr = { B,C,H,G,A,D,E,F,x1,x2,y1,y2,z1,z0,z2 };
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
void Cube3D::translate(Vec3 e)
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
void Cube3D::viewOld(Vec3 e, Vec3 v)
{
	translate(-e);
	// u,v,n axes

	Vec3 n = { 0,0,1 }; //find from a-e vector
	//Normalize v
	Vec3 u = { 1,0,0 }; //Find from VxN
	// here same as x,y,n
	float tranMatrix[4][4] = {
									{u.x,  u.y,   u.z,   0 },
									{v.x,  v.y,   v.z,   0 },
									{n.x,  n.y,   n.z,   0 },
									{0,	   0,     0,     1 }
	};
	transformShape(tranMatrix);
}
void Cube3D::view(Vec3 e, Vec3 v)
{
	// u,v,n axes		
	Vec3 n = { 0,0,1 }; //find from a-e vector
	Vec3 u = { 1,0,0 }; //Find from VxN

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
	float dp = zprp - zvp;
	if (!dp) {
		consoleLog("DP is zero\n");
		dp = 10;
	}
	if (!zprp) {
		consoleLog("Zprp is zero\n"); zprp = 1;
	}
	float tranMatrix[4][4] = {
								  1,         0,       -xprp / dp,       xprp * zvp / dp,
								  0,         1,       -yprp / dp,       yprp * zvp / dp,
								  0,         0,       -zvp / dp,        zvp * (zprp / dp),
								  0,         0,       -1 / dp,          zprp / dp
	};
	float result[4] = { 0 };
	for (int index = 0; index < n; index++) {
		transformPoint(tranMatrix, result, index);
		result[0] /= result[3];	result[1] /= result[3];
		result[2] /= result[3];	result[3] = 1;
		vectorify(result, index);
	}
}
void Cube3D::colorTriangle(Vec3 A, Vec3 B, Vec3 C, Color color, Vec3 off) {
	float dx1, dx2, dx3;
	Vec3 array[] = { A, B, C};
	SortByY(array);
	A = array[0];
	B = array[1];
	C = array[2];
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
void Cube3D::colorFace(Vec3 A, Vec3 B, Vec3 C, Vec3 D, Color color, Vec3 off) {		
	colorTriangle(A, B, C, color, off);
	colorTriangle(C, D, A, color, off);
}
void Cube3D::drawCube(bool colored) {
	Vec3 off{ getMidX(), getMidY(), 0 };
	//B,C,H,G,A,D,E,F
	//0,1,2,3,4,5,6,7

	Color front = 0xcaffbfff, middle = 0x9bf6ffff, back = 0xffffffff;
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
	DrawBresLine(vertSet[8], vertSet[9], 0xff0000ff, off); //x
	DrawBresLine(vertSet[10], vertSet[11], 0xff0000ff, off); //y
	DrawBresLine(vertSet[12], vertSet[13], 0xff0000ff, off); //z
	DrawBresLine(vertSet[14], vertSet[13], 0xff0000ff, off); //z

	//B,C,H,G,A,D,E,F
	//0,1,2,3,4,5,6,7
	if (colored) {
		colorFace(vertSet[0], vertSet[3], vertSet[2], vertSet[1], 0xffadadff, off); //BGCH
		colorFace(vertSet[4], vertSet[0], vertSet[1], vertSet[5], 0xffd6a5ff, off); //ABCD
		colorFace(vertSet[7], vertSet[3], vertSet[2], vertSet[6], 0xfdffb6ff, off); //FGHE
		colorFace(vertSet[5], vertSet[1], vertSet[2], vertSet[6], 0xcaffbfff, off); //CHED
		colorFace(vertSet[4], vertSet[0], vertSet[3], vertSet[7], 0x9bf6ffff, off); //ABGF
		colorFace(vertSet[4], vertSet[7], vertSet[6], vertSet[5], 0xbdb2ffff, off); //AFED
	}
}
void Cube3D::drawCubeOrigin(float x, float y, float z,
	Color front, Color middle, Color back, bool colored) {
	Vec3 off{ x, y, z };
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
	DrawBresLine(vertSet[8], vertSet[9], 0xff0000ff, off); //x
	DrawBresLine(vertSet[10], vertSet[11], 0xff0000ff, off); //y
	DrawBresLine(vertSet[12], vertSet[13], 0xff0000ff, off); //z
	DrawBresLine(vertSet[14], vertSet[13], 0xff0000ff, off); //z
}