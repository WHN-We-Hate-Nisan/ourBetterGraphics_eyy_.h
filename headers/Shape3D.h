#pragma once
#include"Essentials.h"

class Shape3D {
public:
	typedef std::vector<Vect3<float>> VertSet;
	VertSet vertSet;
	int n;
	float tempMatrix[4][max_Vertex] = { 0 };
	float tempMat[4] = { 0 };

	auto matrixify(VertSet);
	auto matrixify(Vect3<float>);
	auto vectorify(float[4], int);
public:
	Shape3D(int);
	Shape3D();
	Shape3D(VertSet);
	void transformShape(float[][4]);
	void transformPoint(float[4][4], float[4], int);
	void draw();
	void translate(Vect3<float>);
	void scale(int);
	void rotateZ(float);
	void rotateX(float);
	void rotateY(float);
	void viewOld(Vect3<float>, Vect3<float> = { 0, 1, 0 });
	void view(Vect3<float>, Vect3<float> = { 0, 1, 0 });
	void orthographic_projection(bool, bool, bool);
	void oblique_projection(float, float);
	void perspective_projection(float, float, float, float);
	void colorTriangle(Vect3<float>, Vect3<float>, Vect3<float>, unsigned int, Vect3<float>);
	void colorFace(Vect3<float>, Vect3<float>, Vect3<float>, Vect3<float>, unsigned int, Vect3<float>);
	void drawCube(bool = false);
	void drawCubeOrigin(float = 0, float = 0, float = 0,
		unsigned int = 0xcaffbf, unsigned int = 0xcaffbf, unsigned int = 0xcaffbf, bool=false);
};

