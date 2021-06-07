#pragma once
#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "CameraController.h"
#include "InputDevice.h"

class DoubleGame :  public Game
{
public:
	Camera* gameCamera = nullptr;
	CameraController* gameCameraCtrl = nullptr;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	//Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	//Microsoft::WRL::ComPtr<IDXGIFactory> factory;

	IDXGIDevice* superDevice;
	

public:
	DoubleGame(DisplayWindow* display1, DisplayWindow* display2, std::string name) : Game(display1, display2, name)
	{
	}

	HRESULT CreateBackBufers();

	virtual HRESULT PrepareRecources() override;
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	

	void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};

