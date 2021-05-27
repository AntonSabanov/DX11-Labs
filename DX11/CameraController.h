#pragma once
//#include "Game.h"//
//#include "Camera.h"//
#include "InputDevice.h"

class Game;
class Camera;

class CameraController
{
public:
	Game* game = nullptr;
	Camera* gameCamera = nullptr;

	float yaw;
	float pitch;

	float velocityMagnitude = 10.0f; //скорость передвижения камеры
	float mouseSensitivity = 1.0f; //скорость врашения камеры

	DirectX::SimpleMath::Vector3 cameraPosition;


public:
	CameraController(Game* gameInst, Camera* camera);

	void Update(float deltaTime);

protected:
	void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
};

