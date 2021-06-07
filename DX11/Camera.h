#pragma once
#include "pch.h"
#include "SimpleMath.h"

class Game;

class Camera
{
	//точка в пространстве
	//матрица проекции перспективная или ортографическая
public:
	Game* game = nullptr;

	DirectX::SimpleMath::Matrix viewMatrix;//где находится и куда смотрит камера
	DirectX::SimpleMath::Matrix projectionMatrix;//как проецируем
public:
	Camera(Game* gameInst);

	DirectX::SimpleMath::Matrix GetCameraMatrix() const;
	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetForwardVector() const;

	DirectX::SimpleMath::Matrix UpdateProjectionMatrix();
};

