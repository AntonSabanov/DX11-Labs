#pragma once
#include "pch.h"
//#include "Camera.h"

class Game;
class Camera;

class GameComponent
{   
public:
    //ID3D11DeviceContext* context;
    Camera* gameCamera = nullptr;   //�� ���� ����� �������� ������� ���� � ��������
    //Vector3 objectPosition;         //������� ������ �������

public:
    virtual HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) = 0;//?
    virtual void Draw(ID3D11DeviceContext* context) = 0; // ����� ����, � ����� ������� �������� � � ��������� �������� ��� ������� ������, ��� - ������� ������� ��� ����������� �����, ��� ������� �����
    //virtual void Reload() = 0;
    virtual void Update(float deltaTime) = 0;//�������� �������� � �������
    virtual void DestroyResources() = 0; 
    virtual void GetGameInstance(Game* inGame) = 0;
};

