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
	Triangle& MultiplyTriangle(Triangle& result, Triangle& in) {
		result.color = in.color;
		for (int i = 0; i < 3; i++)
			result.texCood[i] = in.texCood[i];
		for (int i = 0; i < 3; i++)
			result.vertex[i] = this->MultiplyVector(in.vertex[i]);
		return result;
	}
	static Mat4x4 MakeIdentity() {
		Mat4x4 mat;
		mat.m[0][0] = 1.0f;
		mat.m[1][1] = 1.0f;
		mat.m[2][2] = 1.0f;
		mat.m[3][3] = 1.0f;
		return mat;
	}
	static Mat4x4 MakeRotationZ(float deg) {
		float rad = deg * pi / 180.0f;
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
	static Mat4x4 MakeRotationY(float deg) {
		float rad = -deg * pi / 180.0f;
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
	static Mat4x4 MakeRotationX(float deg) {
		float rad = -deg * pi / 180.0f;
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
	static Mat4x4 PointAt(Vec3& position, Vec3& target, Vec3& up) {
		//Calculate new Forward (n)
		Vec3 n = (target - position).normalize();

		//Calculate new Up (u)
		Vec3 u = up - (n * up.dot(n));

		//New right direction (v)
		Vec3 v = u * n;

		//Dimensioning and Translation Matrix
		Mat4x4 mat;
		mat.m[0][0] = v.x;			mat.m[0][1] = v.y;			mat.m[0][2] = v.z;
		mat.m[1][0] = u.x;			mat.m[1][1] = u.y;			mat.m[1][2] = u.z;
		mat.m[2][0] = n.x;			mat.m[2][1] = n.y;			mat.m[2][2] = n.z;
		mat.m[3][0] = position.x;	mat.m[3][1] = position.y;	mat.m[3][2] = position.z; mat.m[3][3] = 1.0f;
		return mat;
	}
	//Inverse Rotation or Translation Matrix
	static Mat4x4 LookAtInverse(Mat4x4& i) {
		Mat4x4 mat;
		mat.m[0][0] = i.m[0][0];	mat.m[0][1] = i.m[1][0];	mat.m[0][2] = i.m[2][0]; // v.x, u.x, n.x, 0
		mat.m[1][0] = i.m[0][1];	mat.m[1][1] = i.m[1][1];	mat.m[1][2] = i.m[2][1]; // v.y, u.y, n.y, 0
		mat.m[2][0] = i.m[0][2];	mat.m[2][1] = i.m[1][2];	mat.m[2][2] = i.m[2][2]; // v.z, u.z, n.z, 0

		for (int j = 0; j < 3; j++) {
			mat.m[3][j] = 0.0f;
			for (int k = 0; k < 3; k++)
				mat.m[3][j] -= i.m[3][k] * mat.m[k][j];
		}
		mat.m[3][3] = 1.0f; // (v x p) (u x p) (n x p) 1
		return mat;
		/*
		v.x u.x, n.x, 0
		v.y u.y, n.y, 0
		v.z u.z, n.z, 0
		(v x p) (u x p) (n x p) 1
		*/
	}
};
struct Controller {
	bool up = 0, down = 0, left = 0, right = 0,
		forward = 0, backward = 0,
		yawL = 0, yawR = 0;
	void reset() {
		up = 0; down = 0; left = 0; right = 0;
		forward = 0; backward = 0;
		yawL = 0; yawR = 0;
	}
};
class Shape3D {
	Mesh mesh;
	Mat4x4 matProj;
	Texture* texture;

	Vec3 camera{ 0.0f, 0.0f, 0.0f };
	float speed = -1.0f;
	Vec3 lookDir= { 0,0,1 };
	float yaw=0;

	float fTheta=0;

public:
	Shape3D() {
		//The Cube
		mesh.triangles = {
			// SOUTH
			{ 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f,		1.0f, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f},

			// EAST           																	   
			{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f,		1.0f, 0.0f},
			{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f},
																						  
			// NORTH           																		   
			{ 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f,		1.0f, 0.0f},
			{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f},
																						  
			// WEST            																		   
			{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f,		1.0f, 0.0f},
			{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f},
																						  
			// TOP             																		   
			{ 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f,		1.0f, 0.0f},
			{ 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f},
																						  
			// BOTTOM          																	  
			{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f,		1.0f, 0.0f},
			{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f},
		};

		//Loading obj
		//mesh.LoadFromObjectFile("../Assets/Church.obj");
		//mesh.LoadFromObjectFile("../Assets/Cube.obj");
		//mesh.LoadFromObjectFile("../Assets/Teapot.obj");
		//mesh.LoadFromObjectFile("../Assets/Axis.obj");
		
		//For Release
		//mesh.LoadFromObjectFile("Object.obj");

		//Load Texture
		texture = new Texture(L"../Assets/Textures/Jario.spr");

		matProj = Mat4x4::MakeProjection();		
	}
	void checkInput(Controller& c, float elapsedFrames = 0) {
		elapsedFrames *= 0.1f;
		fTheta += elapsedFrames;

		if (c.up) 
			camera.y += speed;
		if (c.down) 
			camera.y -= speed;
		if (c.left) 
			camera.x -= speed;
		if (c.right) 
			camera.x += speed;

		Vec3 forward = lookDir * speed;

		if (c.forward)
			camera -= forward;		;
		if (c.backward)
			camera += forward;
		if (c.yawL)
			yaw -= speed;
		if (c.yawR)
			yaw += speed;
	}
	void draw() {			
		Mesh toRaster;

		// Rotation Z
		Mat4x4 matRotZ;
		matRotZ = Mat4x4::MakeRotationZ(5.0f);
		// Rotation Y
		Mat4x4 matRotY;
		matRotY = Mat4x4::MakeRotationY(fTheta);
		// Rotation X
		Mat4x4  matRotX;
		matRotX = Mat4x4::MakeRotationX(0);
		//Tranlation
		Mat4x4 matTrans;
		matTrans = Mat4x4::MakeTranslate(0.0f, 0.0f, 20.0f);
		//World Transformations
		Mat4x4 matWorld;
		matWorld = Mat4x4::MakeIdentity();
		/*matWorld.MultiplyMatrix(matRotZ);
		matWorld.MultiplyMatrix(matRotY);*/
		matWorld.MultiplyMatrix(matTrans);

		//camera
		Vec3 up = { 0,1,0 };
		Vec3 target = { 0,0,1 };
		Mat4x4 matCamRotate = Mat4x4::MakeRotationY(yaw);
		lookDir = matCamRotate.MultiplyVector(target);
		target = camera + lookDir;

		Mat4x4 matCamera = Mat4x4::PointAt(camera, target, up);

		//View Matrix from camera
		Mat4x4 matView = Mat4x4::LookAtInverse(matCamera);

		//Draw Triangles
		for (auto tri : mesh.triangles) {
			Triangle triProjected, triTransformed, triViewed;

			//Apply Transformations
			matWorld.MultiplyTriangle(triTransformed, tri);

			//Find Normal
			Vec3 normal = triTransformed.normal();
			Vec3 CameraRay = triTransformed.vertex[0] - camera;

			if(normal.dot(CameraRay.normalize()) < 0.0f){
				//Illumination
				Vec3 lightDirection = { 0.0f, 1.0f, -1.0f };
				lightDirection.normalize();

				float light = normal.dot(lightDirection);

				unsigned col = interPolate(-1.0f, 1.0f, light, (unsigned int)0, (unsigned int)0xff);
				triTransformed.color 
					= col * 0x100 * 0x100 +
					  col * 0x100 +
					  col;

				//Convert World Space to View Space
				matView.MultiplyTriangle(triViewed, triTransformed);
				
				//Clip viewed triangle against near plane
				//Forms upto 2 additional triangles
				int nClippedTriangles = 0;
				Triangle clipped[2];
				nClippedTriangles = Triangle::ClipAgainstPlane({ 0.0f,0.0f, 0.1f }, { 0.0f,0.0f, 0.1f }, triViewed, clipped[0], clipped[1]);

				for(int n=0; n<nClippedTriangles;n++) {
					//Projection
					matProj.MultiplyTriangle(triProjected, clipped[n]);

					//Project Textures
					for (int i = 0; i < 3; i++) {
						triProjected.texCood[i] /= triProjected.vertex[i].w;
						triProjected.texCood[i].w = 1.0f / triProjected.vertex[i].w;
					}
					//Normalise Projected matrix
					triProjected.normalize();

					//Scale Triangles into view
					Vec3 offSetView = { 1.0f, 1.0f, 0.0f };
					triProjected += offSetView;
					triProjected *= { 0.5f * (float)globalBuffer.width, 0.5f * (float)globalBuffer.height, 1.0f };

					//Store Triangles
					toRaster.triangles.push_back(triProjected);
				}
			}
		}
		
		//Sort Triangles - Painter's Algorithm
		std::sort(toRaster.triangles.begin(), toRaster.triangles.end(), [](Triangle& t1, Triangle& t2) {
			float midZ1 = (t1.vertex[0].z + t1.vertex[1].z + t1.vertex[2].z) / 3.0f;
			float midZ2 = (t2.vertex[0].z + t2.vertex[1].z + t2.vertex[2].z) / 3.0f;
			return midZ1 > midZ2;
		});

		//Rasterize Triangle
		for (auto& triToRasterize : toRaster.triangles) {
			
			//Clip triangles against all four screen edges
			Triangle clipped[2];
			std::list<Triangle> triangleList;
			triangleList.push_back(triToRasterize);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++) {
				int nTrisToAdd = 0;
				while (nNewTriangles > 0) {
					Triangle test = triangleList.front();
					triangleList.pop_front();
					nNewTriangles--;

					switch (p) {
					//bottom
					case 0: nTrisToAdd = Triangle::ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); 
						break;
					//top
					case 1: nTrisToAdd = Triangle::ClipAgainstPlane({ 0.0f, (float)globalBuffer.height-1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
					//left
					case 2: nTrisToAdd = Triangle::ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
					//right
					case 3: nTrisToAdd = Triangle::ClipAgainstPlane({(float)globalBuffer.width-1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
					}
					for (int w = 0; w < nTrisToAdd; w++)
						triangleList.push_back(clipped[w]);
				}
				nNewTriangles = triangleList.size();
			}
			for (auto& tri : triangleList) {
				for (int i = 0; i < 3; i++) {
					tri.vertex[i].y = (float)globalBuffer.height - tri.vertex[i].y;
					tri.vertex[i].x = (float)globalBuffer.width - tri.vertex[i].x;
				}
				TextureTriangle(tri, texture);
				//ColorTriangle(tri, tri.color);
				DrawTriangle(tri, 0xffffff-tri.color);
			}
		}
	}		
};

