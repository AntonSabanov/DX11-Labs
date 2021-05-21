#pragma once
#include "GameComponent.h"
class TriangleComponent : public GameComponent
{
public:
	Game* game;
	TriangleComponent();
	ID3D11InputLayout* layout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* vertexBC; //���������������� ��������� ������ ���������� ����
	//ID3DBlob* errorVertexCode;
	ID3DBlob* pixelBC; //����� � ������ ��� ���������� ������
	//ID3DBlob* errorPixelCode;
	ID3D11Buffer* vertexBuffer; //vertices

	ID3D11RasterizerState* rastState;

public:

	//void DestroyResources() override;
	void Draw(ID3D11DeviceContext* context) override;
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;
	/*void Reload() override;
	void Update() override;*/
};

