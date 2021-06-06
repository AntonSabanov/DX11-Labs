#pragma once
#include "GameComponent.h"
#include "Camera.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class Game;

class LightObjComponent : public GameComponent
{
public:
	Game* game = nullptr;

	ID3D11DeviceContext* context;

	std::vector<Vector4> triangleObjPoints;				//список координат точек объекта
	std::vector<int> pointIndeces;

	ID3D11InputLayout* layout = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* vertexBC = nullptr;						//скомпилированный вершинный шейдер помещается сюда
	ID3DBlob* pixelBC = nullptr;						//место в памяти под пиксельный шейдер
	ID3D11Buffer* vertexBuffer = nullptr;				//vertices
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11RasterizerState* rastState = nullptr;			//настройка отрисовки для конкретного объекта
	ID3D11SamplerState* samplerState;
	//ID3DUserDefinedAnnotation* annotation = nullptr;

	ID3D11Buffer* constantBuffer = nullptr;				//константный буфер
	Camera* gameCamera = nullptr;						//от сюда будем получать матрицы вида и проекции

	Vector3 objectPosition;								//позиция самого объекта

	DirectX::SimpleMath::Vector4* points = nullptr;

	LPCWSTR objFileName;								//имя 3д модели
	LPCWSTR textureName;								//имя текстуры
	ID3D11Texture2D* texture;							//текстура
	ID3D11ShaderResourceView* texSRV;					//

	ID3D11Buffer* lightBuffer = nullptr;

public:
	LightObjComponent(Game* inGame,
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		Vector3 startPosition,
		std::vector<Vector4> points,
		std::vector<int> indeces,
		LPCWSTR inFileName,
		LPCWSTR inTexName,
		Camera* camera);

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;

	void Draw(ID3D11DeviceContext* context) override;

	//void Reload() override;
	void Update(float deltaTime) override;
	void DestroyResources() override;

private:
	HRESULT CreateShaders(ID3D11Device* device);
	HRESULT CreateLayout(ID3D11Device* device);
	HRESULT CreateBufers(ID3D11Device* device);
};

