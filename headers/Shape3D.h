#pragma once

struct Mesh {
	std::vector<Triangle> triangles;

	bool LoadFromObjectFile(std::string FileName) {
		std::ifstream f(FileName);
		if (!f.is_open()) return false;
		
		//Vertices
		std::vector<Vec3> vertices;

		while (!f.eof()) {
			char line[128];
			f.getline(line, 128);

			std::strstream s;
			s << line;

			char dataType;

			//Getting vertices and making a pool
			if (line[0] == 'v') {
				Vec3 v;
				s >> dataType >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}

			//Getting triangles and making a pool
			if (line[0] == 'f') {
				int f[3];
				s >> dataType >> f[0] >> f[1] >> f[2];
				triangles.push_back({ vertices[f[0] - 1],vertices[f[1] - 1], vertices[f[2] - 1] });
			}
		}

		return true;
	}
};
struct Mat4x4 {
	float m[4][4] = { 0 };
	static void Multiply(Vec3& i, Vec3& o, Mat4x4& m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
			o.x /= w; o.y /= w; o.z /= w;
	}
	Vec3 MultiplyVector(Vec3& i) {
		Vec3 o;
		o.x = i.x * m[0][0] + i.y * m[1][0] + i.z * m[2][0] + i.w * m[3][0];
		o.y = i.x * m[0][1] + i.y * m[1][1] + i.z * m[2][1] + i.w * m[3][1];
		o.z = i.x * m[0][2] + i.y * m[1][2] + i.z * m[2][2] + i.w * m[3][2];
		o.w = i.x * m[0][3] + i.y * m[1][3] + i.z * m[2][3] + i.w * m[3][3];
		return o;
	}
	Mat4x4 MultiplyMatrix(Mat4x4& i) {
		Mat4x4 mat;
		for (int col = 0; col < 4; col++)
			for (int row = 0; row < 4; row++)
				for (int k = 0; k < 4; k++)
					mat.m[row][col] += m[row][k] * i.m[k][col];
		*this = mat;
		return *this;
	}
	Mat4x4 MultiplyMatrix(Mat4x4& i1, Mat4x4& i2) {
		Mat4x4 mat;
		for (int col = 0; col < 4; col++)
			for (int row = 0; row < 4; row++)
				for (int k = 0; k < 4; k++)
					mat.m[row][col] += i1.m[row][k] * i2.m[k][col];
		*this = mat;
		return *this;
	}
	static Mat4x4 MakeIdentity() {
		Mat4x4 mat;
		mat.m[0][0] = 1.0f;
		mat.m[1][1] = 1.0f;
		mat.m[2][2] = 1.0f;
		mat.m[3][3] = 1.0f;
		return mat;
	}
	static Mat4x4 MakeRotationZ(float rad) {
		Mat4x4 mat;
		mat.m[0][0] = cosf(rad);
		mat.m[0][1] = sinf(rad);
		mat.m[1][0] = -sinf(rad);
		mat.m[1][1] = cosf(rad);
		mat.m[2][2] = 1;
		mat.m[3][3] = 1;
		return mat;
		/*
		cos(theeta),	-sin(theeta),	0, 0,
		sin(theeta),	cos(theeta),	0, 0,
		0,				0,				1, 0,
		0,				0,				0, 1
		*/
	}
	static Mat4x4 MakeRotationY(float rad) {
		Mat4x4 mat;
		mat.m[0][0] = cosf(rad);
		mat.m[0][2] = -sinf(rad);
		mat.m[1][1] = 1;
		mat.m[2][0] = sinf(rad);
		mat.m[2][2] = cosf(rad);
		mat.m[3][3] = 1;
		return mat;
		/*
		cos(theeta),       0,       +sin(theeta),      0,
		0,                 1,       0,                 0,
		-sin(theeta),      0,       cos(theeta),       0,
		0,                 0,       0,                 1
		*/
	}
	static Mat4x4 MakeRotationX(float rad) {
		Mat4x4 mat;
		mat.m[0][0] = 1;
		mat.m[1][1] = cosf(rad);
		mat.m[1][2] = sinf(rad);
		mat.m[2][1] = -sinf(rad);
		mat.m[2][2] = cosf(rad);
		mat.m[3][3] = 1;
		return mat;
		/*
		1,              0,              0,              0,
		0,              cos(theeta),    -sin(theeta),   0,
		0,              sin(theeta),    cos(theeta),    0,
		0,              0,              0,				1
		*/
	}
	static Mat4x4 MakeScale(float x, float y, float z) {
		Mat4x4 mat;
		mat.m[0][0] = x;
		mat.m[1][1] = y;
		mat.m[2][2] = x;
		mat.m[3][3] = 1;
		return mat;
		/*
		{x,  0,   0,   0 },
		{0,  x,   0,   0 },
		{0,  0,   x,   0 },
		{0,  0,   0,   1 }
		*/
	}
	static Mat4x4 MakeScale(float s) {
		return MakeScale(s, s, s);
		/*
		{s,  0,   0,   0 },
		{0,  s,   0,   0 },
		{0,  0,   s,   0 },
		{0,  0,   0,   1 }
		*/
	}
	static Mat4x4 MakeTranslate(Vec3 e) {
		Mat4x4 mat;
		mat.m[0][0] = 1;
		mat.m[1][1] = 1;
		mat.m[2][2] = 1;
		mat.m[3][0] = e.x;
		mat.m[3][1] = e.y;
		mat.m[3][2] = e.z;
		mat.m[3][3] = 1;
		return mat;
		/*
		{1,  0,   0,   e.x },
		{0,  1,   0,   e.y },
		{0,  0,   1,   e.z },
		{0,  0,   0,   1   }
		*/
	}
	static Mat4x4 MakeTranslate(float x, float y, float z) {
		return MakeTranslate({ x, y, z });
		/*
		{1,  0,   0,   x },
		{0,  1,   0,   y },
		{0,  0,   1,   z },
		{0,  0,   0,   1   }
		*/
	}
	static Mat4x4 MakeOrthographicProjection(bool x, bool y, bool z) {
		Mat4x4 mat;
		mat.m[0][0] = !x;
		mat.m[1][1] = !y;
		mat.m[2][2] = !z;
		mat.m[3][3] = 1;
		return mat;
		/*
		!x,   0,    0,   0,
		0,    !y,   0,   0,
		0,    0,    !z,  0,
		0,    0,    0,   1
		*/
	}
	static Mat4x4 MakeOrthographicProjection(float alpha, float theeta) {
		if (alpha == 0 || alpha == 90 || alpha == 180 || alpha == 270 || alpha == 360) alpha++;

		Mat4x4 mat;
		theeta *= pi / 180;
		alpha *= pi / 180;
		float l1 = 1 / tan(alpha);

		mat.m[0][0] = 1;
		mat.m[1][1] = 1;
		mat.m[2][0] = l1 * cos(theeta);
		mat.m[2][1] = l1 * sin(theeta);
		mat.m[3][3] = 1;
		return mat;
		/*
		{1, 0, l1 * cos(theeta), 0},
		{0, 1, l1 * sin(theeta), 0},
		{0, 0, 0,				 0},
		{0, 0, 0,				 1}
		*/
	}
	static Mat4x4 MakeProjection(float fNear = 0.1f, float fFar = 1000.0f, float fFovDegrees = 90.0f, float fAspectRatio = 0) {
		if (fAspectRatio == 0) fAspectRatio = (float)globalBuffer.height / (float)globalBuffer.width;
		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * pi);

		Mat4x4 mat;
		mat.m[0][0] = fAspectRatio * fFovRad;
		mat.m[1][1] = fFovRad;
		mat.m[2][2] = fFar / (fFar - fNear);
		mat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		mat.m[2][3] = 1.0f;
		mat.m[3][3] = 0.0f;
		return mat;
	}
};

class Shape3D {
	Mesh meshCube;
	Mat4x4 matProj;
	Vec3 Camera{ 0.0f, 0.0f, 0.0f };
	float fTheta=0;

public:
	Shape3D() {
		//Loading obj
		//meshCube.LoadFromObjectFile("../Assets/Church.obj");
		meshCube.LoadFromObjectFile("../Assets/Cube.obj");
		//Projection Matrix
		matProj = Mat4x4::MakeProjection();		
	}
	void draw(float fElapsedTime = 0) {
		fTheta += fElapsedTime * 0.01f;

		// Rotation Z
		Mat4x4 matRotZ;
		matRotZ = Mat4x4::MakeRotationZ(fTheta);
		// Rotation X
		Mat4x4  matRotX;
		matRotX = Mat4x4::MakeRotationX(fTheta * 0.5f);
		//Tranlation
		Mat4x4 matTrans;
		matTrans = Mat4x4::MakeTranslate(0.0f, 0.0f, 3.0f);
		//World Transformations
		Mat4x4 matWorld;
		matWorld = Mat4x4::MakeIdentity();
		matWorld.MultiplyMatrix(matRotZ, matRotX);
		matWorld.MultiplyMatrix(matTrans);

		Mesh toRaster;
		//Draw Triangles
		for (auto tri : meshCube.triangles) {
			Triangle triProjected, triTransformed;

			//Apply Transformations
			for (int i = 0; i < 3; i++)
				triTransformed.vertex[i]= matWorld.MultiplyVector(tri.vertex[i]);

			//Find Normal
			Vec3 normal = triTransformed.normal();
			Vec3 CameraRay = triTransformed.vertex[0] - Camera;

			if(normal.dot(CameraRay) < 0.0f){
				//Illumination
				Vec3 lightDirection = { 0.0f, 1.0f, -1.0f };
				lightDirection.normalize();

				float light = normal.dot(lightDirection);

				unsigned col = interPolate(-1.0f, 1.0f, light, (unsigned int)0, (unsigned int)0xff);
				triTransformed.color 
					= col * 0x100 * 0x100 +
					  col * 0x100 +
					  col;

				//Projection
				for (int i = 0; i < 3; i++)
					triProjected.vertex[i] = matProj.MultiplyVector(triTransformed.vertex[i]);
				
				//Normalise Projected matrix
				for (int i = 0; i < 3; i++)
					triProjected.vertex[i] = triProjected.vertex[i] / triProjected.vertex[i].w;

				//Scale Triangles into view
				Vec3 offSetView = { 1.0f, 1.0f, 0.0f };
				triProjected += offSetView;
				triProjected.multiply({ 0.5f * (float)globalBuffer.width, 0.5f * (float)globalBuffer.height, 1.0f });
				triProjected.color = triTransformed.color;

				//Store Triangles
				toRaster.triangles.push_back(triProjected);				
			}
		}
		
		//Sort Triangles - Painter's Algorithm
		std::sort(toRaster.triangles.begin(), toRaster.triangles.end(), [](Triangle& t1, Triangle& t2) {
			float midZ1 = (t1.vertex[0].z + t1.vertex[1].z + t1.vertex[2].z) / 3.0f;
			float midZ2 = (t2.vertex[0].z + t2.vertex[1].z + t2.vertex[2].z) / 3.0f;
			return midZ1 > midZ2;
		});

		//Rasterize Triangle
		for (auto& triProjected : toRaster.triangles) {
			ColorTriangle(triProjected, triProjected.color);
			//DrawTriangle(triProjected, 0);
		}
	}	
};

