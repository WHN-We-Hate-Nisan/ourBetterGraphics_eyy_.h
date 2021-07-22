#pragma once

class Cube3D {
public:
	typedef std::vector<Vec3> VertSet;
	VertSet vertSet;
	int n;
	float tempMatrix[4][max_Vertex] = { 0 };
	float tempMat[4] = { 0 };

	auto matrixify(VertSet);
	auto matrixify(Vec3);
	auto vectorify(float[4], int);
public:
	Cube3D(int);
	Cube3D();
	Cube3D(VertSet);
	void transformShape(float[][4]);
	void transformPoint(float[4][4], float[4], int);
	void draw();
	void translate(Vec3);
	void scale(int);
	void rotateZ(float);
	void rotateX(float);
	void rotateY(float);
	void viewOld(Vec3, Vec3 = { 0, 1, 0 });
	void view(Vec3, Vec3 = { 0, 1, 0 });
	void orthographic_projection(bool, bool, bool);
	void oblique_projection(float, float);
	void perspective_projection(float, float, float, float);
	void colorTriangle(Vec3, Vec3, Vec3, unsigned int, Vec3);
	void colorFace(Vec3, Vec3, Vec3, Vec3, unsigned int, Vec3);
	void drawCube(bool = false);
	void drawCubeOrigin(float = 0, float = 0, float = 0,
		unsigned int = 0xcaffbf, unsigned int = 0xcaffbf, unsigned int = 0xcaffbf, bool=false);
};

