#pragma once
#include "pch.h"

class GameComponent
{   
public:
    virtual HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) = 0;//?
    virtual void Draw(ID3D11DeviceContext* context) = 0; // ����� ����, � ����� ������� �������� � � ��������� �������� ��� ������� ������, ��� - ������� ������� ��� ����������� �����, ��� ������� �����
    //virtual void Reload() = 0;
    virtual void Update(float deltaTime) = 0;//�������� �������� � �������
    virtual void DestroyResources() = 0; 
};

