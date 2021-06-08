#pragma once
#include "pch.h"
//#include "Camera.h"

class Game;
class Camera;

struct ObjectTransform
{
    float rotationAngle = 0.0f;
    float scaleKoeff = 1.0f;
};

class GameComponent
{   
public:
    Camera* gameCamera = nullptr;   //от сюда будем получать матрицы вида и проекции


public:
    virtual HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) = 0;//?
    virtual void Draw(ID3D11DeviceContext* context) = 0; // берем гейм, у гейма достаем контекст и у контекста вызываем сет пиксель шейдер, сет - вертекс шейдерб сет растерайзед стейт, сет вертекс буфер
    //virtual void Reload() = 0;
    virtual void Update(float deltaTime) = 0;//апдетить значения в буферах
    virtual void DestroyResources() = 0; 
    virtual void GetGameInstance(Game* inGame) = 0;
};

