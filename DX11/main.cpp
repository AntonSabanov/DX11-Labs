
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

#include "GameTransform.h"
#include "DoubleGame.h"
#include "InputDevice.h"

#define ZCHECK(exp)                                                        \
    if (FAILED(exp)) {                                                     \
        char buf[1000];                                                     \
        sprintf_s(buf, "Check failed at file: %s at line %i\n", __FILE__, __LINE__); \
        printf(buf);\
        OutputDebugStringA(buf); \
        return 0;                                                          \
    }

using namespace DirectX;


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

	DisplayWindow* display1 = new DisplayWindow(1024, 1024);
	DisplayWindow* display2 = new DisplayWindow(1024, 1024);

	//Game* game = new Game(display, "DX11 Framework");
	//InputDevice* inputDevice = new InputDevice(game);
	//game->inputDevice = inputDevice;
	//game->Run();

	//GameTransform* game = new GameTransform(display, "DX11 Framework");
	//InputDevice* inputDevice = new InputDevice(game);
	//game->inputDevice = inputDevice;
	//game->Run();

	DoubleGame* game2 = new DoubleGame(display1, display2, "DX11 Framework");
	InputDevice* inputDevice2 = new InputDevice(game2);
	game2->inputDevice = inputDevice2;
	game2->Run();

}

