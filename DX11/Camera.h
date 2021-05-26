#pragma once
#include "pch.h"
#include "SimpleMath.h"

//using namespace DirectX;

class Camera
{
	//точка в пространстве
	//матрица проекции
public:
	DirectX::SimpleMath::Matrix viewMatrix;//где находится и куда смотрит камера
	DirectX::SimpleMath::Matrix projectionMatrix;//как проецируем
};

