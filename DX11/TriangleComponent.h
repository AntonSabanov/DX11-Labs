#pragma once
#include "pch.h"
#include "GameComponent.h"

#include "Camera.h"//


class TriangleComponent : public GameComponent
{
public:
	std::vector<DirectX::XMFLOAT4> triangleObjPoints; //список координат точек объекта

	ID3D11InputLayout* layout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* vertexBC; //скомпилированный вершинный шейдер помещается сюда
	ID3DBlob* pixelBC; //место в памяти под пиксельный шейдер
	ID3D11Buffer* vertexBuffer; //vertices
	ID3D11Buffer* indexBuffer;
	ID3D11RasterizerState* rastState;
	//ID3DUserDefinedAnnotation* annotation = nullptr;

	ID3D11Buffer* constantBuffer;//константный буфер
	Camera* gameCamera = nullptr;//от сюда будем получать матрицы вида и проекции

	//position//позиция самого объекта

public:
	TriangleComponent(Microsoft::WRL::ComPtr<ID3D11Device> device, 
						ID3D11DeviceContext* context, 
						std::vector<DirectX::XMFLOAT4> points, 
						Camera* camera);

	HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, ID3D11DeviceContext* context) override;

	void Draw(ID3D11DeviceContext* context) override;
	
	//void Reload() override;
	void Update(float deltaTime) override;
	void DestroyResources() override;

private:
	HRESULT CreateShaders(Microsoft::WRL::ComPtr<ID3D11Device> device);
	HRESULT CreateLayout(Microsoft::WRL::ComPtr<ID3D11Device> device);
	HRESULT CreateBufers(Microsoft::WRL::ComPtr<ID3D11Device> device);
};

