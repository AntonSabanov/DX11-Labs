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
	//Game* Instance;
	std::string name;
	DisplayWindow* appDisplay = nullptr;//окно приложения
	D3D11_VIEWPORT viewport = {};
	ID3D11Texture2D* backBuffer = nullptr; //задний буфер в виде текстуры	
	ID3D11Texture2D* depthBuffer = nullptr;
	ID3D11DeviceContext* context = nullptr;//рисование
	ID3DUserDefinedAnnotation* annotation = nullptr; // debug annotation
	ID3D11Device* device;//девайс Microsoft::WRL::ComPtr<ID3D11Device>
	////Instance
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	////RenderSRV
	ID3D11RenderTargetView* rtv = nullptr;//объект заднего буфера, в который будем рисовать //RenderView		//?
	////ScreenResized
	////StartTime
	IDXGISwapChain* swapChain = nullptr;//работа с буферами, вывод на экран
	IDXGISwapChain1* swapChain1 = nullptr;
	float totalTime = 0;
	unsigned int frameCount = 0;

	InputDevice* inputDevice = nullptr;

	ID3D11DepthStencilView* depthView = nullptr;

	TextureLoader* texLoader;

public:
	Game(DisplayWindow* display, std::string name);
	
	HRESULT Run();
	virtual void Initialize(); //создаем энное количество триангл компонентов и добовляем их в components //будет переопределен в классе наследнике, там будут созданы компоненты, компоненты будут добавлены//затем пробигаем по всем компонентам и вызываем у них инишалайз
	virtual HRESULT PrepareRecources();
	HRESULT CreateBackBufer();
	virtual void Draw(); // пробегает по всем компонентам и вызывает у них метод draw
	virtual void DestroyRecources();
	
	virtual void Update(float deltaTime);// пробегает по всем компонентам и вызывает у них метод апдейт
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


};

