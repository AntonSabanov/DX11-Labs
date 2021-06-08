#pragma once
#include "pch.h"
#include "GameComponent.h"
#include "Camera.h"//

#include "SimpleMath.h"


using namespace DirectX::SimpleMath;

class Game;

class GridComponent : public GameComponent
{
public:
	ID3D11DeviceContext* context;
	Game* game = nullptr;

	std::vector<Vector4> objectPoints; //������ ��������� ����� �������
	std::vector<int> pointIndeces;

	ID3D11InputLayout* layout = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* vertexBC = nullptr; //���������������� ��������� ������ ���������� ����
	ID3DBlob* pixelBC = nullptr; //����� � ������ ��� ���������� ������
	ID3D11Buffer* vertexBuffer = nullptr; //vertices
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11RasterizerState* rastState = nullptr; //��������� ��������� ��� ����������� �������
	//ID3DUserDefinedAnnotation* annotation = nullptr;

	ID3D11Buffer* constantBuffer = nullptr;//����������� �����
	//Camera* gameCamera = nullptr;//�� ���� ����� �������� ������� ���� � ��������

	Vector3 objectPosition;//������� ������ �������

	DirectX::SimpleMath::Matrix worldMatrix;//

public:
	GridComponent(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vector4> points, std::vector<int> indeces, Camera* camera);
	GridComponent(Game* inGame, std::vector<Vector4> points, std::vector<int> indeces);

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;//?
	void Draw(ID3D11DeviceContext* context) override; // ����� ����, � ����� ������� �������� � � ��������� �������� ��� ������� ������, ��� - ������� ������� ��� ����������� �����, ��� ������� �����
	//virtual void Reload() override;
	void Update(float deltaTime) override;//�������� �������� � �������
	void DestroyResources() override;
	void GetGameInstance(Game* inGame) override;
//
private:
	HRESULT CreateShaders(ID3D11Device* device);
	HRESULT CreateLayout(ID3D11Device* device);
	HRESULT CreateBufers(ID3D11Device* device);
};

