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
    virtual void Draw(ID3D11DeviceContext* context) = 0; // ����� ����, � ����� ������� �������� � � ��������� �������� ��� ������� ������, ��� - ������� ������� ��� ����������� �����, ��� ������� �����
    virtual HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) = 0;//?
    virtual void Reload() = 0;
    virtual void Update() = 0;//�������� �������� � �������
};

