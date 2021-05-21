
#pragma warning(disable : 4267)


#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <iostream>
#include "winuser.h"
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXColors.h>
#include <chrono>
#include <thread>

#include "DisplayWindow.h"
#include "Game.h"

#define ZCHECK(exp)                                                        \
    if (FAILED(exp)) {                                                     \
        char buf[1000];                                                     \
        sprintf_s(buf, "Check failed at file: %s at line %i\n", __FILE__, __LINE__); \
        printf(buf);\
        OutputDebugStringA(buf); \
        return 0;                                                          \
    }

using namespace DirectX;

//оюъ€вление глобальных переменных
//ID3D11DeviceContext* context;//рисование
//IDXGISwapChain* swapChain;//работа с буферами, вывод на экран
//IDXGISwapChain1* swapChain1;
//ID3D11RenderTargetView* rtv;//объект заднего буфера, в который будем рисовать
//ID3DUserDefinedAnnotation* annotation;

//DisplayWindow* applicationWindow;

//std::chrono::time_point<std::chrono::steady_clock> PrevTime;
//float totalTime = 0;
//unsigned int frameCount = 0;

//вызываетс€ каждый раз, когда система получает сообщение (нужно дл€ окошка)
//LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
//{
//	//auto id = std::this_thread::get_id();
//	//std::cout << "WndProc id: " << id << "\n";
//
//	switch (umessage)
//	{
//		// Check if the window is being destroyed.
//	case WM_DESTROY:
//	case WM_CLOSE:
//	{
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	case WM_SIZE:
//	{
//		std::cout << "Width " << LOWORD(lparam) << " Height " << HIWORD(lparam) << std::endl;
//
//		return 0;
//	}
//
//	// Check if a key has been pressed on the keyboard.
//	case WM_KEYDOWN:
//	{
//		// If a key is pressed send it to the input object so it can record that state.
//		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
//
//		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
//		return 0;
//	}
//
//	// Check if a key has been released on the keyboard.
//	case WM_KEYUP:
//	{
//		// If a key is released then send it to the input object so it can unset the state for that key.
//		return 0;
//	}
//
//	// All other messages pass to the message handler in the system class.
//	default:
//	{
//		return DefWindowProc(hwnd, umessage, wparam, lparam);
//	}
//	}
//}
/////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow);
int main (){
	return WinMain({}, {}, {}, {});
}
///////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//----------------------------------------------------------------
	//CREATE A FORM
	//----------------------------------------------------------------	
	DisplayWindow* display = new DisplayWindow(800, 800);

	Game* game = new Game(display);
	game->Run();

}

