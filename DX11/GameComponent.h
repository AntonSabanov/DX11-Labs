#pragma once
#include "pch.h"
#include "Game.h"

class GameComponent
{
public:
    //Game* game = nullptr;
public:
	//GameComponent();
    virtual void DestroyResources() = 0;
    virtual void Draw(ID3D11DeviceContext* context) = 0; // берем гейм, у гейма достаем контекст и у контекста вызываем сет пиксель шейдер, сет - вертекс шейдерб сет растерайзед стейт, сет вертекс буфер
    virtual HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) = 0;//?
    virtual void Reload() = 0;
    virtual void Update() = 0;//апдетить значения в буферах
};

