#pragma once
#include "GameComponent.h"
#include "Camera.h"//

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class GridComponent : public GameComponent
{
//public:
//	std::vector<Vector4> triangleObjPoints; //список координат точек объекта
//
//	ID3D11InputLayout* layout;
//	ID3D11VertexShader* vertexShader;
//	ID3D11PixelShader* pixelShader;
//	ID3DBlob* vertexBC; //скомпилированный вершинный шейдер помещается сюда
//	ID3DBlob* pixelBC; //место в памяти под пиксельный шейдер
//	ID3D11Buffer* vertexBuffer; //vertices
//	ID3D11Buffer* indexBuffer;
//	ID3D11RasterizerState* rastState;
//	//ID3DUserDefinedAnnotation* annotation = nullptr;
//
//	ID3D11Buffer* constantBuffer;//константный буфер
//	Camera* gameCamera = nullptr;//от сюда будем получать матрицы вида и проекции
//
//	//position//позиция самого объекта
//
public:
	GridComponent(ID3D11Device* device,
		ID3D11DeviceContext* context,
		std::vector<Vector4> points,
		Camera* camera);

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;//?
	void Draw(ID3D11DeviceContext* context) override; // берем гейм, у гейма достаем контекст и у контекста вызываем сет пиксель шейдер, сет - вертекс шейдерб сет растерайзед стейт, сет вертекс буфер
	//virtual void Reload() override;
	void Update(float deltaTime) override;//апдетить значения в буферах
	void DestroyResources() override;
//
//private:
//	HRESULT CreateShaders(ID3D11Device* device);
//	HRESULT CreateLayout(ID3D11Device* device);
//	HRESULT CreateBufers(ID3D11Device* device);
};

