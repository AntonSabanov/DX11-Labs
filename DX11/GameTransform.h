#pragma once
#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "CameraController.h"

class GameTransform : public Game
{
public:
	Camera* gameCamera = nullptr;
	CameraController* gameCamCtrl = nullptr;

public:
	//GameTransform();

	virtual void Initialize() override;
	//HRESULT PrepareRecources() override;
	virtual void Update(float deltaTime) override;
	//void Draw() override;
	//virtual void DestroyRecources() override;
};

