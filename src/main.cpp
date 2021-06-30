#include"ourBetterGraphics_eyy_.h"

void resetMatrix(float matrix[4][max_Vertex], int n) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0.0;
}
void displaymatrix(float mat[4][max_Vertex], int n) {
	consoleLog("Matrix Display\n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < n; j++) {
			consoleLogSpace(mat[i][j]);
		}
		consoleLog("\n");
	}
	consoleLog("\n");
}
void Products(float matA[][4], float matB[][max_Vertex], float result[][max_Vertex], int n)
{
	resetMatrix(result, n);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < 4; k++)
				result[i][j] += matA[i][k] * matB[k][j];
}




//float result[4][100] = { 0 };
//
//void product(float first[4][4], float second[4][100], int n)
//{
//	float temp[4][100] = { 0 };
//	for (int i = 0; i < 4; i++)
//		for (int j = 0; j < n; j++)
//			for (int k = 0; k < 4; k++)
//				temp[i][j] += first[i][k] * second[k][j];
//	for (int i = 0; i < 4; i++)
//		for (int j = 0; j < n; j++)
//			result[i][j] = temp[i][j];
//}
//void translate(int x, int y, int z, float vertices[4][100], int n)
//{
//	float Composite[4][4] = {
//								{1,  0,     0,     x},
//								{0,  1,     0,      y},
//								{0,  0,     1,      z},
//								{0,  0,     0,      1 }
//	};
//	product(Composite, vertices, n);
//}
//void rotationz(float theeta, float vertices[4][100], int n)
//{
//	theeta = theeta * (pi / 180);
//	float Composite[4][4] = {
//								cos(theeta),    -sin(theeta),   0,  0,
//								sin(theeta),    cos(theeta),    0,  0,
//								0,              0,              1,  0,
//								0,              0,              0,  1
//	};
//	product(Composite, vertices, n);
//}
//void rotationx(float theeta, float vertices[4][100], int n)
//{
//	theeta = theeta * (pi / 180);
//	float Composite[4][4] = {
//								1,              0,              0,              0,
//								0,              cos(theeta),    -sin(theeta),   0,
//								0,              sin(theeta),    cos(theeta),    0,
//								0,              0,              0,				1
//	};
//	product(Composite, vertices, n);
//}
//void rotationy(float theeta, float vertices[4][100], int n)
//{
//	theeta = theeta * (pi / 180);
//	float Composite[4][4] = {
//								  cos(theeta),       0,       sin(theeta),       0,
//								  0,                 1,       0,                 0,
//								  -sin(theeta),      0,       cos(theeta),       0,
//								  0,                 0,       0,                 1
//	};
//	product(Composite, vertices, n);
//}
//void orthographic_projection(float vertices[4][100], int n, bool x, bool y, bool z)
//{
//	float Composite[4][4] = {
//								!x,     0,          0,      0,
//								0,      !y,         0,      0,
//								0,      0,          !z,     0,
//								0,      0,          0,      1
//	};
//	product(Composite, vertices, n);
//}
//void perspective_projection(float zprp, float zvp, float vertices[4][100], int n)
//{
//	float dp = zprp - zvp;
//	float Composite[4][4] = {
//								  1,         0,       0,            0,
//								  0,         1,       0,            0,
//								  0,         0,       -zvp / dp,      zvp * (zprp / dp),
//								  0,         0,       -1 / dp,        zprp / dp
//	};
//
//	product(Composite, vertices, n);
//	for (int i = 0; i < n; i++)
//	{
//		result[0][i] /= result[3][i];
//		result[1][i] /= result[3][i];
//		result[2][i] /= result[3][i];
//		result[3][i] = 1;
//	}
//	//for (int i = 0; i < n; i++)
//	//{
//	//	cout << result[0][i] << "   " << result[0][i] << "    " << result[0][i] << endl;
//	//}
//}
//void Cube(int n, unsigned color)
//{
//	for (int i = 0; i < 4; i++)
//		DDAlgorithm(result[0][i % 4], result[1][i % 4], result[0][(i + 1) % 4], result[1][(i + 1) % 4], color);
//	for (int i = 0; i < 4; i++)
//		DDAlgorithm(result[0][4 + i % 4], result[1][4 + i % 4], result[0][4 + (i + 1) % 4], result[1][4 + (i + 1) % 4], color);
//	for (int i = 0; i < 4; i++)
//		DDAlgorithm(result[0][i], result[1][i], result[0][4 + i], result[1][4 + i], color);
//}
//void Cube_3D(float mat[4][100], int n, unsigned color)
//{
//	translate(getMaxX(), getMaxY(), 0, mat, n);
//	Cube(n, color);	
//}


//void Transformations() {
//	int x1 = 80, x2 = 60, x3 = 55, y1 = 90, y2 = 105, y3 = 90;
//	int nx1, nx2, nx3, ny1, ny2, ny3;
//	drawBresLine(Bect2{ 500, 0 }, Bect2{ 500, 800 }, 0x90ee90);
//	drawBresLine(Bect2{ 0, 400 }, Bect2{ 1000, 400 }, 0x90ee90);
//	float point[3][3] = { {x1,x2,x3},{y1,y2,y3},{1,1,1} };
//	float newPoint[3][3] = {};
//	drawBresLine(Bect2{ x1 + 500, y1 + 400 }, Bect2{ x2 + 500, y2 + 400 }, 0x90ee99);
//	drawBresLine(Bect2{ x2 + 500, y2 + 400 }, Bect2{ x3 + 500, y3 + 400 }, 0x90ee99);
//	drawBresLine(Bect2{ x3 + 500 , y3 + 400 }, Bect2{ x1 + 500, y1 + 400 }, 0x90ee99);
//
//	//1.Translation with specified parameters
//	translation(point, newPoint, Vect2{ 25,30 }); drawTriangle(newPoint, 0xffadad);
//	//2.Rotation with specified parameters
//	rotation(point, newPoint, 150); drawTriangle(newPoint, 0xffd6a5);
//	//3.Scaling with specified parameters
//	scaling(point, newPoint, Vect2{ 2.6, 1.3 }); drawTriangle(newPoint, 0xfdffb6);
//	//4.Rotation about a pivot point
//	rotationPivot(point, newPoint, 45, Bect2{ -100,50 }); drawTriangle(newPoint, 0xcaffbf);
//	//5.Scaling about a fixed point
//	scalingFixed(point, newPoint, Bect2{ 35, -80 }, Vect2{ 2.6, 1.3 }); drawTriangle(newPoint, 0x9bf6ff);
//	//6.Scaling with orthogonal axis at certain	angle from x - axis
//	scalingWithAngle(point, newPoint, 60, Vect2{ 1.6, 2.1 }); drawTriangle(newPoint, 0x9bf6ff);
//	//7.Reflection with specified axis
//	reflection(point, newPoint, 1); drawTriangle(newPoint, 0xa0c4ff);
//	//reflection(point, newPoint,2); drawTriangle(newPoint);
//	//reflection(point, newPoint,3); drawTriangle(newPoint);
//	//reflection(point, newPoint,4); drawTriangle(newPoint);
//
////8.Shearing with specified parameter
//	shearing(point, newPoint, 3, Vect2{ -1.2, 0.5 }); drawTriangle(newPoint, 0xbdb2ff);
//	//shearing(point, newPoint, 2, Vect2{ -1.2, 0.5 }); drawTriangle(newPoint);
//	//shearing(point, newPoint, 1, Vect2{ -1.2, 0.5 }); drawTriangle(newPoint);
//
////9.Shearing with specified reference line
//	shearigRef(point, newPoint, 1, 2, 2); drawTriangle(newPoint, 0xffc6ff);
//}

LRESULT CALLBACK WindowProc(HWND window_handle, unsigned int message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_DESTROY:
	case WM_QUIT:
	{
		globalRunning = false;
	}break;
	default:
		result = DefWindowProc(window_handle, message, w_param, l_param);
	}

	return result;
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) 
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

	if (RegisterClass(&wndclass)) {
		HWND window_handle = CreateWindowEx(0, wndclass.lpszClassName, L"Hello69", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, globalBuffer.width, globalBuffer.height,
			0, 0, hInstance, 0);
		if (window_handle) {
			globalRunning = true;
			MSG message;
			HDC deviceContext = GetDC(window_handle);

			int angle=0;
			int i = 21;
			int flag = 1;

			while (globalRunning)
			{
				while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&message);
					DispatchMessageA(&message);
				}

#pragma endregion
				clrScr();
				
				//translate(getMaxX(), getMaxY(), 0, mat, n);
				//rotationy(30, result, n);
				//rotationx(30, result, n);

				Shape3D p1;
				p1.rotateX(45);
				p1.rotateY(angle);
				p1.rotateZ(angle);
				//p1.translate(getMidX(), getMidY(), i);
				//p1.oblique_projection(60, 20);
				p1.drawCube();

				/*
				Vect3<float> off(getMaxX(), getMaxY(), 0);
				//B,C,H,G,A,D,E,F
				//0,1,2,3,4,5,6,7
				
				auto vertSet = p1.vertSet;

				unsigned int front = 0xcaffbf, middle = 0x9bf6ff, back = 0xffffff;
				
				//Back
				drawBresLine(vertSet[0], vertSet[3], back, off); //BG
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[3], vertSet[2], back, off); //GH
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[2], vertSet[1], back, off); //HC
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[1], vertSet[0], back, off); //CB
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);


				//Middle
				drawBresLine(vertSet[4], vertSet[0], middle, off); //AB
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[3], vertSet[7], middle, off); //FG
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[5], vertSet[1], middle, off); //DC
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[6], vertSet[2], middle, off); //EH
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);


				//Front
				drawBresLine(vertSet[4], vertSet[7], front, off); //AF
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[7], vertSet[6], front, off); //FE
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[6], vertSet[5], front, off); //ED
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);

				drawBresLine(vertSet[5], vertSet[4], front, off); //DA
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);
				*/
				
				angle++;
				angle %= 360;
				i += flag;
				if (i >= 160 || i <= 20) flag *= -1;
				//Transformations();
#pragma region Initializer
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);
				Sleep(20);
			}
		}
	}
	return 0;
#pragma endregion
};
