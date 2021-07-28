#pragma once
struct Mesh {
	std::vector<Triangle> triangles;
	typedef std::vector<Vec3> VertSet;
	struct Vertex
	{
		Vec3 position;
		Vec2 texcoord;
		Vec3 normal;
	};
	struct VertRef
	{
		VertRef(int v, int vt, int vn) : v(v), vt(vt), vn(vn) { }
		int v, vt, vn;
	};

	bool LoadFromObjectFile(std::string FileName) {
		std::ifstream f(FileName);
		if (!f.is_open()) return false;
		
		std::vector<Vertex> verts;
		//Vertices
		VertSet vertices(1, { 0,0,0,1 });
		std::vector<Vec2> texcoords(1, { 0,0,0 });
		VertSet normals(1, { 0,0,0,1 });

		while (!f.eof()) {
			std::string line;
			std::getline(f, line);

			std::istringstream lineSS(line);
			std::string lineType;
			lineSS >> lineType;

			/*std::strstream s;
			s << line;*/

			/*char dataType;
			int junk;*/

			/*if (line[0] == 'v') {
				if (line[1] != 'n') {
					Vec3 v;
					s >> dataType >> v.x >> v.y >> v.z;
					vertices.push_back(v);
				}
			}*/
			//Getting vertices and making a pool
			if (lineType == "v")
			{
				float x = 0, y = 0, z = 0, w = 1;
				lineSS >> x >> y >> z >> w;
				vertices.push_back(Vec3{ x, y, z, w });
			}

			//Getting textures and making a pool
			if (lineType == "vt")
			{
				float u = 0, v = 0, w = 0;
				lineSS >> u >> v >> w;
				texcoords.push_back(Vec2{ u, v, w });
			}

			//Getting normals and making a pool
			if (lineType == "vn")
			{
				float i = 0, j = 0, k = 0;
				lineSS >> i >> j >> k;
				normals.push_back(Vec3{ i, j, k }.normalize());
			}

			if (lineType == "f")
			{
				std::vector<VertRef> refs;
				std::string refStr;
				while (lineSS >> refStr) {
					std::istringstream ref(refStr);
					std::string vStr, vtStr, vnStr;
					std::getline(ref, vStr, '/');
					std::getline(ref, vtStr, '/');
					std::getline(ref, vnStr, '/');
					int v = atoi(vStr.c_str());
					int vt = atoi(vtStr.c_str());
					int vn = atoi(vnStr.c_str());
					v = (v >= 0 ? v : vertices.size() + v);
					vt = (vt >= 0 ? vt : texcoords.size() + vt);
					vn = (vn >= 0 ? vn : normals.size() + vn);
					refs.push_back(VertRef(v, vt, vn));
				}

				// triangulate, assuming n>3-gons are convex and coplanar
				for (size_t i = 1; i + 1 < refs.size(); ++i) {
					const VertRef* p[3] = { &refs[0], &refs[i], &refs[i + 1] };

					Vec3 U = vertices[p[1]->v] - vertices[p[0]->v];
					Vec3 V = vertices[p[2]->v] - vertices[p[0]->v];
					Vec3 faceNormal = (U * V).normalize();

					for (size_t j = 0; j < 3; ++j) {
						Vertex vert;
						vert.position = vertices[p[j]->v];
						vert.texcoord = texcoords[p[j]->vt];
						vert.normal = (p[j]->vn != 0 ? normals[p[j]->vn] : faceNormal);
						verts.push_back(vert);
					}
				}
			}			
		}

		//Getting triangles and making a pool
		consoleLogSpace(verts.size());
		for (int i = 0; i < verts.size() / 3; i++) {
			Triangle tri;
			for (int j = 0; j < 3; j++) {
				tri.vertex[j] = verts[i * 3 + j].position;
				tri.texCood[j] = verts[i * 3 + j].texcoord;
				tri.normals[j] = verts[i * 3 + j].normal;
			}
			triangles.push_back(tri);
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
			result.normals[i] = in.normals[i];
		for (int i = 0; i < 3; i++)
			result.intensities[i] = in.intensities[i];
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
	static Mat4x4 MakeObliqueProjection(float alpha, float theeta) {
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
	     lUp = 0, lDown = 0, lLeft = 0, lRight = 0,
		lForward = 0, lBackward = 0,
		forward = 0, backward = 0,
		yawL = 0, yawR = 0, colored = 1, wireframe = 0, shaded=1;

	void reset() {
		up = 0; down = 0; left = 0; right = 0;
		lUp = 0; lDown = 0; lLeft = 0; lRight = 0;
		lForward = 0; lBackward = 0;
		forward = 0; backward = 0;
		yawL = 0; yawR = 0;
	}
};
class Shape3D {
	Mesh mesh;
	Mat4x4 matProj;
	Texture* texture;

	Vec3 camera{ 0.0f, 0.0f, 0.0f };

	//Lighting Parameters
	Vec3 lightDirection = { 1.0f, 1.0f, 1.0f };
	Vec3 lightPosition = { 5.0f, 5.0f,5.0f };
	float Ka = 0.75f, Kd = 0.75f, Ks = 0.5f,
		Ia = 5.0f, Il = 7.0f;
	int n = 10;

	float speed = 50.0f;
	Vec3 lookDir= { 0,0,1 };
	float yaw=0;

	float fTheta=0;

	bool wireframe = 0, colored = 1, shaded=1;
public:
	Shape3D() {
		//Loading Light
		mesh.LoadFromObjectFile("../Assets/cubeSmall.obj");

		//The Cube
		//mesh.triangles = {
		//	// SOUTH
		//	{ 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f},
		//	{ 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f},
		//																																				  
		//	// EAST           																	   		  												 
		//	{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f},
		//	{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f},
		//																				  			
		//	// NORTH           																		 
		//	{ 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f},
		//	{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f},
		//																				  				
		//	// WEST            																		   	
		//	{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f},
		//	{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f},
		//																				  				  
		//	// TOP             																		   	   
		//	{ 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f},
		//	{ 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f},
		//																				  				 
		//	// BOTTOM          																	  		
		//	{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f},
		//	{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f},
		//};

		//Loading obj
		//mesh.LoadFromObjectFile("../Assets/Church.obj");
		//mesh.LoadFromObjectFile("../Assets/Cube2.obj");
		//mesh.LoadFromObjectFile("../Assets/Teapot.obj");
		//mesh.LoadFromObjectFile("../Assets/Axis.obj");
		mesh.LoadFromObjectFile("../Assets/Mountain2.obj");
		//mesh.LoadFromObjectFile("../Assets/Sample.obj");
		
		//For Release
		//mesh.LoadFromObjectFile("Light.obj");
		//mesh.LoadFromObjectFile("Object.obj");

		//Load Texture
		texture = NULL;
		//texture = new Texture("../Assets/Textures/house.png");

		matProj = Mat4x4::MakeProjection();		
	}
	void checkInput(Controller& c, float elapsedTime = 0) {
		elapsedTime *= 0.000001f;
		//fTheta += elapsedFrames;
		float change = speed * elapsedTime;
		if (c.up) 
			camera.y += change;
		if (c.down) 
			camera.y -= change;
		if (c.left) 
			camera.x += change; 
		if (c.right) 
			camera.x -= change;

		if (c.lUp){
			lightDirection.y += change;
			lightPosition.y += change;
		}
		if (c.lDown) {
			lightDirection.y -= change;
			lightPosition.y -= change;
		}
		if (c.lLeft) {
			lightDirection.x += change;
			lightPosition.x += change;
		}
		if (c.lRight) {
			lightDirection.x -= change;
			lightPosition.x -= change;
		}
		if (c.lBackward) {
			lightDirection.z -= change;
			lightPosition.z -= change;
		}
		if (c.lForward) {
			lightDirection.z += change;
			lightPosition.z += change;
		}

		Vec3 forward = lookDir * change;

		if (c.forward)
			camera += forward;		
		if (c.backward)
			camera -= forward;
		if (c.yawL)
			yaw -= change * 10.0f;
		if (c.yawR)
			yaw += change * 10.0f;
		wireframe = c.wireframe;
		colored = c.colored;
		shaded = c.shaded;
	}
	void draw() {
		Mesh toRaster;

		// Rotation Z
		//Mat4x4 matRotZ;
		//matRotZ = Mat4x4::MakeRotationZ(fTheta*0.5f);
		// Rotation Y
		//Mat4x4 matRotY;
		//matRotY = Mat4x4::MakeRotationY(fTheta);
		// Rotation X
		//Mat4x4  matRotX;
		//matRotX = Mat4x4::MakeRotationX(0);
		//Tranlation
		Mat4x4 matTrans;
		matTrans = Mat4x4::MakeTranslate(0.0f, 0.0f, 3.0f);
		Mat4x4 matLightTrans;
		matLightTrans = Mat4x4::MakeTranslate(lightPosition);
		//World Transformations
		Mat4x4 matWorld;
		Mat4x4 matWorld2;
		matWorld = Mat4x4::MakeIdentity();
		matWorld2 = Mat4x4::MakeIdentity();
		//matWorld.MultiplyMatrix(matRotZ);
		//matWorld.MultiplyMatrix(matRotY);
		matWorld.MultiplyMatrix(matTrans);
		matWorld2.MultiplyMatrix(matLightTrans);
		matWorld2.MultiplyMatrix(matTrans);

		//camera
		Vec3 up = { 0,1,0 };
		Vec3 target = { 0,0,1 };
		Mat4x4 matCamRotate = Mat4x4::MakeRotationY(yaw);
		lookDir = matCamRotate.MultiplyVector(target);
		target = camera + lookDir;

		Mat4x4 matCamera = Mat4x4::PointAt(camera, target, up);

		//View Matrix from camera
		Mat4x4 matView = Mat4x4::LookAtInverse(matCamera);

		int sizee = mesh.triangles.size();

		//Draw Triangles
		for (int index=0; index < sizee; index++) {
		//for (auto tri : mesh.triangles) {
			Triangle tri = mesh.triangles[index];
			Triangle triProjected, triTransformed, triViewed;

			//Apply Transformations
			if(index<12)
				matWorld2.MultiplyTriangle(triTransformed, tri);
			else
				matWorld.MultiplyTriangle(triTransformed, tri);

			//Find Normal
			Vec3 normal = triTransformed.normal();
			Vec3 CameraRay = triTransformed.vertex[0] - camera;

			if (Vec3::dot(normal,CameraRay.normalize()) < 0.0f) {
				if (index < 12) {
					triTransformed.color
						= Color(0xff, 0xff, 0xff, 0xff);
					for (int k = 0; k < 3; k++)			
						triTransformed.intensities[k] = 12.5f;
				}
				else {
					//Illumination				

					for (int k = 0; k < 3; k++) {
						Vec3 L = (lightPosition - triTransformed.vertex[k]).normalize();
						Vec3 V = (camera - triTransformed.vertex[k]).normalize();
						Vec3 N = triTransformed.normals[k];

						triTransformed.intensities[k] = Ka * Ia + Kd * Il * Vec3::dot(N, L) + Ks * Il * pow(Vec3::dot(N, (L + V).normalize()), n);
					}
					
					if(!shaded){
					float inten = (triTransformed.intensities[0] + triTransformed.intensities[1] + triTransformed.intensities[2]) / 3;
					inten = (inten < 0) ? 0 : inten;
					float minIp = 0, maxIp = 12.5f;
					unsigned int minCol = 0, maxCol = 0xff;
					unsigned char col = interPolate(minIp, maxIp, inten, minCol, maxCol);
					triTransformed.color
						= Color(col, col, col, 0xff);
					}
				}			

				//Convert World Space to View Space
				matView.MultiplyTriangle(triViewed, triTransformed);

				//Clip viewed triangle against near plane
				//Forms upto 2 additional triangles
				int nClippedTriangles = 0;
				Triangle clipped[2];
				nClippedTriangles = Triangle::ClipAgainstPlane({ 0.0f,0.0f, 0.1f }, { 0.0f,0.0f, 0.1f }, triViewed, clipped[0], clipped[1]);

				for (int n = 0; n < nClippedTriangles; n++) {
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
					case 1: nTrisToAdd = Triangle::ClipAgainstPlane({ 0.0f, (float)globalBuffer.height - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
						//left
					case 2: nTrisToAdd = Triangle::ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
						//right
					case 3: nTrisToAdd = Triangle::ClipAgainstPlane({ (float)globalBuffer.width - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
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
				//TextureTriangle(tri, texture);
				if (colored) { 
					if(shaded) ShadeTriangle(tri);
					else ColorTriangle(tri, tri.color);
				}
				//if (wireframe) DrawTriangle(tri, 0xffffffff - tri.color.color);
				if (wireframe) DrawTriangle(tri, 0xffffffff - tri.color.color);
			}
		}
	}

};

