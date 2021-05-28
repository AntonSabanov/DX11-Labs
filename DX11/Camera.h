#pragma once
#include "pch.h"
//#include "Game.h"
#include "SimpleMath.h"

//using namespace DirectX;

class Game;

class Camera
{
	//����� � ������������
	//������� �������� ������������� ��� ���������������
public:
	Game* game = nullptr;

	DirectX::SimpleMath::Matrix viewMatrix;//��� ��������� � ���� ������� ������
	DirectX::SimpleMath::Matrix projectionMatrix;//��� ����������
public:
	Camera(Game* gameInst);

	DirectX::SimpleMath::Matrix GetCameraMatrix() const;
	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetForwardVector() const;

	DirectX::SimpleMath::Matrix UpdateProjectionMatrix();
	//void UpdateProjectionMatrix();
};

