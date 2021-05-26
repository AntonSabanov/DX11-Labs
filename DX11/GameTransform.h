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

	InputDevice* inputDevice;//перенести в game

public:
	//GameTransform();

	virtual void Initialize() override;	
	virtual void Update(float deltaTime) override;

	//HRESULT PrepareRecources() override;
	//void Draw() override;
	//virtual void DestroyRecources() override;
};

