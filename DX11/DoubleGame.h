#pragma once
#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "CameraController.h"
#include "InputDevice.h"
#include "GameTransform.h"

class DoubleGame //:  public Game
{
public:
	/*Camera* gameCamera = nullptr;
	CameraController* gameCameraCtrl = nullptr;*/
	ID3D11Device* device = nullptr;//global
	ID3D11DeviceContext* context = nullptr;//global

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	//Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	//Microsoft::WRL::ComPtr<IDXGIFactory> factory;

	IDXGIDevice* superDevice;
	
	std::vector<GameTransform*> gameWindows;

public:
	DoubleGame(std::vector<GameTransform*> games);

	HRESULT CreateBackBufers();
	HRESULT PrepareRecources();
	HRESULT DoubleRun();

	void Initialize();
	//void Update(float deltaTime);

	void Draw(); // пробегает по всем компонентам и вызывает у них метод draw
	void DestroyRecources();

	//virtual HRESULT PrepareRecources() override;
	//virtual void Initialize() override;
	//virtual void Update(float deltaTime) override;
	

	void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};

