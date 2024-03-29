#include "DisplayWindow.h"
#include "Game.h"

//----------------------------------------------------------------
//CREATE A FORM
//----------------------------------------------------------------	

DisplayWindow::DisplayWindow(int width, int height)
{
	hInst = GetModuleHandle(nullptr);

	LPCWSTR applicationName;

	//inDevice = new InputDevice();

	// Give the application a name.
	applicationName = L"Game";

	WNDCLASSEX wc;
	int posX, posY;

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = Game::WndProc;//DisplayWindow::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// If windowed then set it to 800x800 resolution.
	screenWidth = width;
	screenHeight = height;

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	RECT windowRect = { 0, 0, static_cast<LONG>(screenWidth), static_cast<LONG>(screenHeight) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME; // WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
	// Create the window with the screen settings and get the handle to it.
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInst, nullptr);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
};

//LRESULT CALLBACK DisplayWindow::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)//���������� ������ ���, ����� ������� �������� ��������� (����� ��� ������)
//{
//	switch (umessage)
//	{
//		// Check if the window is being destroyed.
//		case WM_DESTROY:
//		case WM_CLOSE:
//		{
//			PostQuitMessage(0);
//			//isExitRequested = true;
//			return 0;
//		}
//
//		case WM_SIZE:
//		{
//			std::cout << "Width " << LOWORD(lparam) << " Height " << HIWORD(lparam) << std::endl;
//
//			return 0;
//		}
//
//		// Check if a key has been pressed on the keyboard.
//		case WM_KEYDOWN:
//		{
//			// If a key is pressed send it to the input object so it can record that state.
//			std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
//
//			if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
//			return 0;
//		}
//
//		// Check if a key has been released on the keyboard.
//		case WM_KEYUP:
//		{
//			// If a key is released then send it to the input object so it can unset the state for that key.
//			return 0;
//		}
//		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		case WM_INPUT:
//		{
//			UINT dwSize = 0;
//			GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
//			LPBYTE lpb = new BYTE[dwSize];
//			if (lpb == nullptr) {
//				return 0;
//			}
//
//			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
//				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
//
//			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);
//
//			if (raw->header.dwType == RIM_TYPEKEYBOARD)
//			{
//				//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
//				//	raw->data.keyboard.MakeCode,
//				//	raw->data.keyboard.Flags,
//				//	raw->data.keyboard.Reserved,
//				//	raw->data.keyboard.ExtraInformation,
//				//	raw->data.keyboard.Message,
//				//	raw->data.keyboard.VKey);
//
//				inDevice->OnKeyDown({
//					raw->data.keyboard.MakeCode,
//					raw->data.keyboard.Flags,
//					raw->data.keyboard.VKey,
//					raw->data.keyboard.Message
//					});
//			}
//			else if (raw->header.dwType == RIM_TYPEMOUSE)
//			{
//				//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
//				inDevice->OnMouseMove({
//					raw->data.mouse.usFlags,
//					raw->data.mouse.usButtonFlags,
//					static_cast<int>(raw->data.mouse.ulExtraInformation),
//					static_cast<int>(raw->data.mouse.ulRawButtons),
//					static_cast<short>(raw->data.mouse.usButtonData),
//					raw->data.mouse.lLastX,
//					raw->data.mouse.lLastY
//					});
//			}
//
//			delete[] lpb;
//			return DefWindowProc(hwnd, umessage, wparam, lparam);
//		}
//		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		// All other messages pass to the message handler in the system class.
//		default:
//		{
//			return DefWindowProc(hwnd, umessage, wparam, lparam);
//		}
//	}
//}

