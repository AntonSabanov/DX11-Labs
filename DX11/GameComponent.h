#pragma once
#include "pch.h"
//#include "Game.h"

//class Game;

class GameComponent
{
public:
    //Game* game = nullptr;
   
    
public:
    //GameComponent(Game* gameInstance)
   // {
   //   game = gameInstance();
   // }
	//virtual ~GameComponent() = default;
    virtual HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, ID3D11DeviceContext* context) = 0;//?
    virtual void Draw(ID3D11DeviceContext* context) = 0; // берем гейм, у гейма достаем контекст и у контекста вызываем сет пиксель шейдер, сет - вертекс шейдерб сет растерайзед стейт, сет вертекс буфер
    //virtual void Reload() = 0;
    virtual void Update(float deltaTime) = 0;//апдетить значения в буферах
    virtual void DestroyResources() = 0; 
};

