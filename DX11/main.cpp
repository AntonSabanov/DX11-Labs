
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

#include "GridComponent.h"
#include "TextureObjComponent.h"
#include "LightObjComponent.h"

#define ZCHECK(exp)                                                        \
    if (FAILED(exp)) {                                                     \
        char buf[1000];                                                     \
        sprintf_s(buf, "Check failed at file: %s at line %i\n", __FILE__, __LINE__); \
        printf(buf);\
        OutputDebugStringA(buf); \
        return 0;                                                          \
    }

using namespace DirectX;

GridComponent* grid;				//сетка
GridComponent* basis;				//нулевой пивот
TriangleComponent* triangleObject;	//объект из треугольников
TextureObjComponent* texObj;		//объект с текстурой
LightObjComponent* texLightObj;		//объект со светом

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

	//нулевой пивот
	basis = new GridComponent(nullptr, {
		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //ось х
		Vector4(1.0f,  0.0f,  0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), //ось у
		Vector4(0.0f,  1.0f,  0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), //ось z
		Vector4(0.0f,  0.0f,  1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), },
		{ 0, 1, 2, 3, 4, 5 });

	//сетка
	grid = new GridComponent(nullptr, {
		Vector4(-4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), //вертикальные линии
		Vector4(-4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-3.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-3.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-2.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-2.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-1.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-1.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(2.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(2.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(3.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(3.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), //горизонтальные 
		Vector4(4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, -2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, -3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		},
		{ 0,1, 2,3, 4,5, 6,7, 8,9, 10,11, 12,13, 14,15, 16,17, 18,19, 20,21, 22,23, 24,25, 26,27, 28,29, 30,31, 32,33, 34,35 });

	//кубик
	triangleObject = new TriangleComponent(nullptr,{
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		}, { //массив индексов для кубика
		0,1,2, 1,0,3, 4,2,5, 2,4,0, 6,5,7, 5,6,4, 3,7,1, 7,3,6, 4,3,0, 3,4,6, 2,7,5, 7,2,1 });

	//кубик с текстурой
	texObj = new TextureObjComponent(nullptr,(Vector3::Zero + Vector3(0, -1, 0)), {
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 0.0f),		//0
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f),		//1
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 0.0f, 0.0f),		//2
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 0.0f, 0.0f),		//3
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 0.0f, 0.0f),		//4
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f),		//5
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 0.0f),		//6
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 0.0f), },		//7
		{ 0,1,2, 1,0,3, 4,2,5, 2,4,0, 6,5,7, 5,6,4, 3,7,1, 7,3,6, 4,3,0, 3,4,6, 2,7,5, 7,2,1 },//массив индексов для кубика
		L"WallTex3.png");

	//как в примере, но с другими координатами
	texLightObj = new LightObjComponent(nullptr, (Vector3::Zero + Vector3(0, 1, 0)), {
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f), },
		{ 3,1,0, 2,1,3, 6,4,5, 7,4,6, 11,9,8, 10,9,11, 14,12,13, 15,12,14, 19,17,16, 18,17,19, 22,20,21, 23,20,22 },
		L"WallTex.png");

	//Game* game = new Game(display, "DX11 Framework");
	//InputDevice* inputDevice = new InputDevice(game);
	//game->inputDevice = inputDevice;
	//game->Run();

	//GameTransform* game = new GameTransform(display, "DX11 Framework");
	//InputDevice* inputDevice = new InputDevice(game);
	//game->inputDevice = inputDevice;
	//game->Run();


	GameTransform* game1 = new GameTransform(display1, "Left", nullptr, nullptr, { basis, grid, triangleObject });//левое окно
	GameTransform* game2 = new GameTransform(display2, "Right", nullptr, nullptr, { basis, grid, texObj });//правое окно
	//for (size_t i = 0; i < components.size(); ++i)
	//{
	//	components[i]->game = context;
	//	components[i]->gameCamera = gameCamera;
	//}
	InputDevice* inputDevice = new InputDevice(game1);
	//InputDevice* inputDevice2 = new InputDevice(game2);
	game1->inputDevice = inputDevice;
	game2->inputDevice = inputDevice;

	DoubleGame* doubleGame = new DoubleGame({ game1, game2 });
	doubleGame->DoubleRun();
	//game2->Run();

}

