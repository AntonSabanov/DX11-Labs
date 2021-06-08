#pragma once
#include "pch.h"
#include "DisplayWindow.h"
#include "GameComponent.h"
#include "TriangleComponent.h"
#include "TextureLoader.h"
#include <vector>

#include "InputDevice.h"

class Game
{
public:
	std::vector<GameComponent*> components;
	std::string name;

	DisplayWindow* appDisplay = nullptr;//окно приложения
	//DisplayWindow* appDisplay2 = nullptr;//окно приложения

	D3D11_VIEWPORT viewport = {};
	//D3D11_VIEWPORT viewport2 = {};
	//D3D11_VIEWPORT viewports[2] = { viewport, viewport2 };

	ID3D11Texture2D* backBuffer = nullptr; //задний буфер в виде текстуры	
	//ID3D11Texture2D* backBuffer2 = nullptr; //задний буфер в виде текстуры	

	ID3D11Texture2D* depthBuffer = nullptr;
	ID3D11DeviceContext* context = nullptr;//рисование
	ID3DUserDefinedAnnotation* annotation = nullptr; // debug annotation
	ID3D11Device* device;//девайс Microsoft::WRL::ComPtr<ID3D11Device>
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;

	ID3D11RenderTargetView* rtv = nullptr;//объект заднего буфера, в который будем рисовать //RenderView		//?
	//ID3D11RenderTargetView* rtv2 = nullptr;//объект заднего буфера, в который будем рисовать //RenderView		//?
	//ID3D11RenderTargetView* targets[2] = { rtv, rtv2 };

	IDXGISwapChain* swapChain = nullptr;//работа с буферами, вывод на экран
	//IDXGISwapChain* swapChain2 = nullptr;

	IDXGISwapChain1* swapChain1 = nullptr;
	//IDXGISwapChain1* swapChain_2 = nullptr;

	float totalTime = 0;
	unsigned int frameCount = 0;
	InputDevice* inputDevice = nullptr;
	ID3D11DepthStencilView* depthView = nullptr;
	TextureLoader* texLoader;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIDevice* superDevice;

public:
	Game(DisplayWindow* display, std::string name);
	Game(DisplayWindow* display1, DisplayWindow* display2, std::string name);
	Game(DisplayWindow* display, std::string name, ID3D11Device* globalDevice, ID3D11DeviceContext* globalContext, std::vector <GameComponent*> gameObjects);
	//Game(std::vector<GameTransform*>);
	
	HRESULT Run();
	//HRESULT DoubleRun();
	HRESULT CreateLocalRecources();
	virtual void Initialize(); //создаем энное количество триангл компонентов и добовляем их в components //будет переопределен в классе наследнике, там будут созданы компоненты, компоненты будут добавлены//затем пробигаем по всем компонентам и вызываем у них инишалайз
	virtual HRESULT PrepareRecources();
	HRESULT CreateBackBufer();
	virtual void Draw(); // пробегает по всем компонентам и вызывает у них метод draw
	virtual void DestroyRecources();	
	virtual void Update(float deltaTime);// пробегает по всем компонентам и вызывает у них метод апдейт
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

