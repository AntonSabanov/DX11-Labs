#pragma once
#include "Game.h"
#include "Camera.h"
#include "InputDevice.h"

class CameraController
{
public:
	Game* game = nullptr;
	Camera* gameCamera = nullptr;

	float yaw;
	float pitch;

	float velocityMagnitude = 1.0f; //�������� ������������ ������
	float mouseSensitivity = 0.5f; //�������� �������� ������

	DirectX::SimpleMath::Vector3 cameraPosition;


public:
	CameraController(Game* gameInst, Camera* camera);

	void Update(float deltaTime);

protected:
	void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
};

