#pragma once
#include "pch.h"
#include "SimpleMath.h"

//using namespace DirectX;

class Camera
{
	//����� � ������������
	//������� ��������
public:
	DirectX::SimpleMath::Matrix viewMatrix;//��� ��������� � ���� ������� ������
	DirectX::SimpleMath::Matrix projectionMatrix;//��� ����������
};

