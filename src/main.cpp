#pragma once
#include "ourBetterGraphics_eyy_.h"
#include<chrono>

LRESULT CALLBACK WindowProc(HWND window_handle, unsigned int message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_DESTROY:
	case WM_QUIT:
	{
		globalRunning = false;
	}
	break;
	default:
		result = DefWindowProc(window_handle, message, w_param, l_param);
	}

	return result;
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
#pragma region Initializer
	WNDCLASS wndclass = {};
	wndclass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszMenuName = L"HelloThere";
	wndclass.lpszClassName = L"ButGoodbye";

	globalBuffer.width = 800;
	globalBuffer.height = 700;

	globalBuffer.info.bmiHeader = {};
	globalBuffer.info.bmiHeader.biSize = sizeof(globalBuffer.info.bmiHeader);
	globalBuffer.info.bmiHeader.biWidth = globalBuffer.width;
	globalBuffer.info.bmiHeader.biHeight = -globalBuffer.height;
	globalBuffer.info.bmiHeader.biPlanes = 1;
	globalBuffer.info.bmiHeader.biBitCount = 32;
	globalBuffer.info.bmiHeader.biCompression = BI_RGB;

	globalBuffer.memory = VirtualAlloc(0, globalBuffer.width * globalBuffer.height * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (RegisterClass(&wndclass))
	{
		HWND window_handle = CreateWindowEx(0, wndclass.lpszClassName, L"HelloThere", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 
								CW_USEDEFAULT, globalBuffer.width, globalBuffer.height,
								0, 0, hInstance, 0);
		if (window_handle)
		{
			globalRunning = true;
			MSG message;
			HDC deviceContext = GetDC(window_handle);

#pragma endregion
			int angle = 0;
			int i = 0;
			int flag = 1;
			int frameCounter = 0;

			//int x = 91, y = 101, z = -239, p = 171;
			float x = 1, y = 1, z = 201, p = 171;
			bool textured = false;
			Vect3<int> rot{0, 0, 0}, rotBool{0, 0, 0};
			Controller controller;
			unsigned deltatime;
			auto lastframe = std::chrono::high_resolution_clock::now();
			Shape3D s;
#pragma region Loop

			while (globalRunning)
			{
				deltatime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - lastframe).count();
				lastframe = std::chrono::high_resolution_clock::now();
				consoleLogSpace(1e6 / deltatime);
				controller.reset();
				while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
				{
					int multiplier = 10;
					unsigned int vkCode = message.wParam;
					TranslateMessage(&message);
					DispatchMessageA(&message);
					switch (message.message)
					{
					case WM_KEYDOWN:
						if (vkCode == 'W')
						{
							y -= multiplier;
							controller.up = true;
						}
						if (vkCode == 'S')
						{
							y += multiplier;
							controller.down = true;
						}
						if (vkCode == 'A')
						{
							x -= multiplier;
							controller.left = true;
						}
						if (vkCode == 'D')
						{
							x += multiplier;
							controller.right = true;
						}
						if (vkCode == 'Q')
						{
							z -= multiplier;
						}
						if (vkCode == 'E')
						{
							z += multiplier;
						}
						if (vkCode == 'Z')
						{
							p -= multiplier;
							controller.wireframe = !controller.wireframe;
						}
						if (vkCode == 'X')
						{
							p -= multiplier;
							controller.colored = !controller.colored;
						}
						if (vkCode == 'C')
						{
							p += multiplier;
							controller.shaded = !controller.shaded;
						}

						if (vkCode == 'J')
						{
							rot.x += multiplier / 2;
							rotBool = {1, 0, 0};
							controller.yawL = true;
						}
						if (vkCode == 'L')
						{
							rot.x -= multiplier / 2;
							rotBool = {1, 0, 0};
							controller.yawR = true;
						}
						if (vkCode == 'I')
						{
							rot.y += multiplier / 2;
							rotBool = {0, 1, 0};
							controller.forward = true;
						}
						if (vkCode == 'K')
						{
							rot.y -= multiplier / 2;
							rotBool = {0, 1, 0};
							controller.backward = true;
						}
						if (vkCode == 'U')
						{
							rot.z += multiplier / 2;
							rotBool = {0, 0, 1};
						}
						if (vkCode == 'O')
						{
							rot.z -= multiplier / 2;
							rotBool = {0, 0, 1};
						}
						if (vkCode == 'B') {
							textured = !textured;
						}
						if (vkCode == 'T') {
							controller.lUp = 1;
						}
						if (vkCode == 'F') {
							controller.lLeft = 1;
						}
						if (vkCode == 'G') {
							controller.lDown = 1;
						}
						if (vkCode == 'H') {
							controller.lRight = 1;
						}
						if (vkCode == 'R') {
							controller.lBackward = 1;
						}
						if (vkCode == 'Y') {
							controller.lForward = 1;
						}
						break;
					default:
						break;
					}
				}					
#pragma endregion
				ClrScr(Color(173, 225, 229, 0xff));
				//Lab5(x, y, z, p, textured, rot, rotBool);				
				s.checkInput(controller, deltatime);
				s.draw();
				/*Image test("../Assets/Textures/house.png");
				DrawImage(test);*/
#pragma region Initializer
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
							  0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);
				frameCounter++;
				//Sleep(17);
			}
		}
	}
	return 0;
#pragma endregion
};
void Lab5(float x, float y, float z, float p,
			bool textured,
			Vect3<int> rot, Vect3<int> rotBool) {
	//My Beautiful Baby
	Cube3D p1;
	Cube3D p2;
	Cube3D p3;
	Cube3D p4;

	//AntiGimbal Rotation
/*if (rotBool.y)
{
	p1.rotateY(rot.y);
	p1.rotateX(rot.x);
	p1.rotateZ(rot.z);
}
else if (rotBool.x) {
	p1.rotateX(rot.x);
	p1.rotateY(rot.y);
	p1.rotateZ(rot.z);
}
else if (rotBool.z) {
	p1.rotateZ(rot.z);
	p1.rotateY(rot.y);
	p1.rotateX(rot.x);
}*/

	//Assignment 1
	//p1.scale(100);
	p1.rotateX(40);

	//Assignment 2
	//p1.orthographic_projection(0, 0, 1);
	//p1.oblique_projection(10, 60);
	//p1.perspective_projection(-100, 50);

	//Assignment3 - No
	/*p1.translate(-50, -50, 0);
	p1.perspective_projection(- 100, 50);
	p1.translate(getMidX(), getMidY(), 0);*/

	//Assignment4
	/*	p1.translate({ -50, -50, -50 });
	p1.perspective_projection(0, 0, 250, 300);
	p2.translate({ -50, -50, -50 });
	p2.perspective_projection(0, 0, 0, 200);
*/
/*p3.translate({ -50, -50, -50 });
p3.perspective_projection(21, 31, 251, 1);
p4.translate({ -50, -50, -50 });
p4.perspective_projection(1, 1, 310, 1);*/

//Assignment 5
//p1.translate({-50, -50, -50});
//p1.view({x, y, z});


//int x = 91, y = 101, z = -239, p = 171;
//p1.perspective_projection(0, 0, -200, 250);
	p1.drawCube(textured);
}