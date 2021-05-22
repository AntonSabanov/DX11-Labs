#pragma once
#include "pch.h"
#include "GameComponent.h"


class TriangleComponent : public GameComponent
{
public:
	std::vector<DirectX::XMFLOAT4> trianglePoints;
	//Game* game;
	ID3D11InputLayout* layout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* vertexBC; //скомпилированный вершинный шейдер помещается сюда
	//ID3DBlob* errorVertexCode;
	ID3DBlob* pixelBC; //место в памяти под пиксельный шейдер
	//ID3DBlob* errorPixelCode;
	ID3D11Buffer* vertexBuffer; //vertices
	ID3D11Buffer* indexBuffer;

	ID3D11RasterizerState* rastState;

public:
	TriangleComponent(Microsoft::WRL::ComPtr<ID3D11Device> device, ID3D11DeviceContext* context, std::vector<DirectX::XMFLOAT4> points);
	HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, ID3D11DeviceContext* context) override;
	HRESULT CreateShaders(Microsoft::WRL::ComPtr<ID3D11Device> device);
	HRESULT CreateLayout(Microsoft::WRL::ComPtr<ID3D11Device> device);
	HRESULT CreateBufers(Microsoft::WRL::ComPtr<ID3D11Device> device);
	void Draw(ID3D11DeviceContext* context) override;
	
	/*void Reload() override;
	void Update() override;*/
	void DestroyResources() override;
};

