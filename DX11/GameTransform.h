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
	}

	virtual void Initialize() override;	
	virtual void Update(float deltaTime) override;

	void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};

