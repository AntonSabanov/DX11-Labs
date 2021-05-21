#pragma once
#include "pch.h"
#include "DisplayWindow.h"
#include "GameComponent.h"

class Game
{
public:
	DisplayWindow* appDisplay = nullptr;//окно приложения
	//GameComponent* components;
	ID3D11Texture2D* backBuffer = nullptr; //задний буфер в виде текстуры										//?
	ID3D11DeviceContext* context = nullptr;//рисование
	ID3DUserDefinedAnnotation* annotation = nullptr; // debug annotation
	Microsoft::WRL::ComPtr<ID3D11Device> device;//девайс
	////Instance
	////Name
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	////RenderSRV
	ID3D11RenderTargetView* rtv = nullptr;//объект заднего буфера, в который будем рисовать //RenderView		//?
	////ScreenResized
	////StartTime
	IDXGISwapChain* swapChain = nullptr;//работа с буферами, вывод на экран
	IDXGISwapChain1* swapChain1 = nullptr;
	float totalTime = 0;
	unsigned int frameCount = 0;

public:
	Game(DisplayWindow* display);

	HRESULT Run();
	//virtual void Initialize(); //создаем энное количество триангл компонентов и добовляем их в components //будет переопределен в классе наследнике, там будут созданы компоненты, компоненты будут добавлены//затем пробигаем по всем компонентам и вызываем у них инишалайз
	virtual HRESULT PrepareRecources();
	virtual void DestroyRecources();
	HRESULT CreateBackBufer();
	virtual void Update();// пробегает по всем компонентам и вызывает у них метод апдейт
	virtual void Draw(); // пробегает по всем компонентам и вызывает у них метод draw

	//static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


};

