#pragma once
#include "Texture.h"
#include "Mesh.h"
#include "Mat4x4.h"
#include "GeneratedCube.h"

struct Controller {
	bool up = 0, down = 0, left = 0, right = 0,
		lUp = 0, lDown = 0, lLeft = 0, lRight = 0,
		lForward = 0, lBackward = 0,
		forward = 0, backward = 0,
		yawL = 0, yawR = 0, colored = 1, wireframe = 0, shaded = 1;

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

	//Vec3 camera{ 0.0f, 0.0f, 0.0f };
	Vec3 camera{ 0.0f, 15.0f, -15.0f };

	//Lighting Parameters
	Vec3 lightDirection = { 1.0f, 1.0f, 1.0f };
	//Vec3 lightPosition = { 5.0f, 5.0f,5.0f };
	Vec3 lightPosition = { 0.0f, 5.0f, 5.0f };
	float Ka = 0.75f, Kd = 0.75f, Ks = 0.5f,
		Ia = 5.0f, Il = 7.0f;
	int n = 10;

	float speed = 10.0f;
	Vec3 lookDir = { 0,0,1 };
	float yaw = 0;

	float fTheta = 0;

	bool wireframe = 0, colored = 1, shaded = 1;
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
		//mesh.LoadFromObjectFile("../Assets/Mountain2.obj");
		//mesh.LoadFromObjectFile("../Assets/Sample.obj");

		//Marching Cubes
		GeneratedCube marchingCube(32.0f, 2.0f);
		marchingCube.triangles;
		mesh.triangles.insert(mesh.triangles.end(), marchingCube.triangles.begin(), marchingCube.triangles.end());
		
		GeneratedCube waterCube(32.0f, 2.0f, 2, 3);
		marchingCube.triangles;
		mesh.triangles.insert(mesh.triangles.end(), waterCube.triangles.begin(), waterCube.triangles.end());
		
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

		if (c.lUp) {
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

		//Tranlation
		Mat4x4 matTrans;
		matTrans = Mat4x4::MakeTranslate(0.0f, 0.0f, 3.0f);
		Mat4x4 matLightTrans;
		matLightTrans = Mat4x4::MakeTranslate(lightPosition);
		//World Transformations
		Mat4x4 matWorld;
		Mat4x4 matLighting;
		matWorld = Mat4x4::MakeIdentity();
		matLighting = Mat4x4::MakeIdentity();
		matWorld.MultiplyMatrix(matTrans);
		matLighting.MultiplyMatrix(matLightTrans);
		matLighting.MultiplyMatrix(matTrans);

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
		for (int index = 0; index < sizee; index++) {
			Triangle tri = mesh.triangles[index];			
			Triangle triProjected, triTransformed, triViewed;

			//Apply Transformations
			if (index < 12)
				matLighting.MultiplyTriangle(triTransformed, tri);
			else
				matWorld.MultiplyTriangle(triTransformed, tri);

			//Find Normal
			Vec3 normal = triTransformed.normal();
			Vec3 CameraRay = triTransformed.vertex[0].position - camera;

			//Illumination
			/*for (int k = 0; k < 3; k++)
				triTransformed.vertex[k].color
				= Color(0xff, 0xff, 0xff, 0xff);*/
			//if (Vec3::dot(normal, CameraRay.normalize()) < 0.0f) {
			{
				if (index < 12) {
					for (int k = 0; k < 3; k++)
						triTransformed.vertex[k].color = Color(0xff,0xff,0,0xff);
					for (int k = 0; k < 3; k++)
						triTransformed.vertex[k].intensity = 12.5f;
				}
				else {
					/*for (int k = 0; k < 3; k++) {
						triTransformed.vertex[k].color.r = (triTransformed.vertex[k].position.x + 80.f) * 0xff / 160.0f;
						triTransformed.vertex[k].color.g = (triTransformed.vertex[k].position.y) * 0xff / 34.2f;
						triTransformed.vertex[k].color.b = (triTransformed.vertex[k].position.z + 80.f) * 0xff / 160.0f;
					}*/
					for (int k = 0; k < 3; k++) {
						Vec3 L = (lightPosition - triTransformed.vertex[k].position).normalize();
						Vec3 V = (camera - triTransformed.vertex[k].position).normalize();
						Vec3 N = triTransformed.vertex[k].normal;

						triTransformed.vertex[k].intensity = Ka * Ia + Kd * Il * Vec3::dot(N, L) + Ks * Il * pow(Vec3::dot(N, (L + V).normalize()), n);
						triTransformed.vertex[k].intensity = (triTransformed.vertex[k].intensity < 0) ? 0 : triTransformed.vertex[k].intensity;
					}

					if (!shaded) {
						float inten = (triTransformed.vertex[0].intensity + triTransformed.vertex[1].intensity + triTransformed.vertex[2].intensity) / 3;
						
						float minIp = 0, maxIp = 12.5f;
						unsigned int minCol = 0, maxCol = 0xff;
						unsigned char col = interPolate(minIp, maxIp, inten, minCol, maxCol);
						for (int k = 0; k < 3; k++)
							triTransformed.vertex[k].color
							= Color(col, col, col, 0xff);
					}
				}				
				//Convert World Space to View Space
				matView.MultiplyTriangle(triViewed, triTransformed);

				//Clip viewed triangle against near plane
				//Forms upto 2 additional triangles
				int nClippedTriangles = 0;
				Triangle clipped[2];
				nClippedTriangles = ClipAgainstPlane({ 0.0f,0.0f, 0.1f }, { 0.0f,0.0f, 0.1f }, triViewed, clipped[0], clipped[1]);

				for (int n = 0; n < nClippedTriangles; n++) {
					//Projection
					matProj.MultiplyTriangle(triProjected, clipped[n]);

					//Project Textures
					for (int i = 0; i < 3; i++) {
						triProjected.vertex[i].textureCood /= triProjected.vertex[i].position.w;
						triProjected.vertex[i].textureCood.w = 1.0f / triProjected.vertex[i].position.w;
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
			float midZ1 = (t1.vertex[0].position.z + t1.vertex[1].position.z + t1.vertex[2].position.z) / 3.0f;
			float midZ2 = (t2.vertex[0].position.z + t2.vertex[1].position.z + t2.vertex[2].position.z) / 3.0f;
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
					case 0: nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
						//top
					case 1: nTrisToAdd = ClipAgainstPlane({ 0.0f, (float)globalBuffer.height - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
						//left
					case 2: nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
						//right
					case 3: nTrisToAdd = ClipAgainstPlane({ (float)globalBuffer.width - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
					}
					for (int w = 0; w < nTrisToAdd; w++)
						triangleList.push_back(clipped[w]);
				}
				nNewTriangles = triangleList.size();
			}
			for (auto& tri : triangleList) {
				for (int i = 0; i < 3; i++) {
					tri.vertex[i].position.y = (float)globalBuffer.height - tri.vertex[i].position.y;
					tri.vertex[i].position.x = (float)globalBuffer.width - tri.vertex[i].position.x;
				}
				//TextureTriangle(tri, texture);
				if (colored) {
					if (shaded) ShadeTriangle(tri);
					else ColorTriangle(tri, tri.avgColor());
				}
				//if (wireframe) DrawTriangle(tri, 0xffffffff - tri.color.color);
				if (wireframe) DrawTriangle(tri, 0xffffffff - tri.avgColor().color);
			}
		}
	}

};

