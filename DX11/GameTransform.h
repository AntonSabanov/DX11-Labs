#pragma once
#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "CameraController.h"
#include "InputDevice.h"

class GameTransform : public Game
{
public:
	Camera* gameCamera = nullptr;
	CameraController* gameCameraCtrl = nullptr;

public:
	GameTransform(DisplayWindow* display, std::string name) : Game(display, name)
	{
	};
	GameTransform(DisplayWindow* display, std::string name, ID3D11Device* globalDevice, ID3D11DeviceContext* globalContext, std::vector <GameComponent*> gameObjects) 
		: Game(display, name, globalDevice, globalContext, gameObjects)
	{
	};

	virtual void Initialize() override;	
	virtual void Update(float deltaTime) override;

	void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};

