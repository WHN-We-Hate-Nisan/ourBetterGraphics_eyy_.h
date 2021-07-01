
#include"ourBetterGraphics_eyy_.h"

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
				ClrScr();
				
				//translate(getMaxX(), getMaxY(), 0, mat, n);
				//rotationy(30, result, n);
				//rotationx(30, result, n);
				
				//My Beautiful Baby
				Shape3D p1;
				
				//Assignment 1

				p1.drawCubeOrigin();
				/*Shape2D pram1(200, angle, 10);
				pram1.DrawShape();*/

				angle++;
				angle %= 360;
				i += flag;
				if (i >= 160 || i <= 20) flag *= -1;
				//Transformations();
#pragma region Initializer
				StretchDIBits(deviceContext, 0, 0, globalBuffer.width, globalBuffer.height,
					0, 0, globalBuffer.width, globalBuffer.height, globalBuffer.memory, &globalBuffer.info, DIB_RGB_COLORS, SRCCOPY);
				//Sleep(20);
			}
		}
	}
	return 0;
#pragma endregion
};
