#include "LightObjComponent.h"
#include "pch.h"
#include "SimpleMath.h"
#include "Game.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

//struct ConstantData
//{
//	Matrix WorldViewProj;
//	Matrix World;
//	Vector4 ViewerPos;
//};
//
//struct LightData
//{
//	Vector4 Direction;
//	//Vector4 Color;
//	Vector4 KaSpecPowKsX;
//};

struct ConstantData 
{
	Matrix WorldViewProj;
	Matrix World;
	Vector4 ViewerPos;
	Vector4 Direction;
	Vector4 KaSpecPowKsX;
};

LightObjComponent::LightObjComponent(Game* inGame, ID3D11Device* device, ID3D11DeviceContext* context, Vector3 startPosition, std::vector<Vector4> points, std::vector<int> indeces, LPCWSTR inFileName, LPCWSTR inTexName, Camera* camera)
{
	game = inGame;

	for (size_t i = 0; i < points.size(); ++i)
	{
		triangleObjPoints.emplace_back(points[i]);
	}
	for (size_t i = 0; i < indeces.size(); ++i)			//���������� ������� ��������
	{
		pointIndeces.emplace_back(indeces[i]);
	}
	LightObjComponent::context = context;
	gameCamera = camera;
	objectPosition = startPosition;						//��������� ������� �������

	objFileName = inFileName;
	textureName = inTexName;

	Initialize(device, context);						//����� �������������� ������
}

LightObjComponent::LightObjComponent(Game* inGame, Vector3 startPosition, std::vector<Vector4> points, std::vector<int> indeces, LPCWSTR inTexName)
{
	game = inGame;

	for (size_t i = 0; i < points.size(); ++i)
	{
		triangleObjPoints.emplace_back(points[i]);
	}
	for (size_t i = 0; i < indeces.size(); ++i)			//���������� ������� ��������
	{
		pointIndeces.emplace_back(indeces[i]);
	}
	objectPosition = startPosition;						//��������� ������� �������

	textureName = inTexName;

	//Initialize(device, context);						//����� �������������� ������
}

void LightObjComponent::GetGameInstance(Game* inGame)
{
	game = inGame;
}

HRESULT LightObjComponent::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT res;

	CreateShaders(device);
	CreateLayout(device);
	CreateBufers(device);

	//-----------------------------------------------------------------------------
	//SETUP RASTERIZER STAGE AND VIEWPORT
	//-----------------------------------------------------------------------------
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;		// �� ������ �������� ����������� �������� ������������ (NONE - �� ����� �����������, BACK - ����������� ������,FRONT)
	rastDesc.FillMode = D3D11_FILL_SOLID;		//������� ��������� �������� (SOLID - ����������, WIREFRAME - ������ �����)
	rastDesc.FrontCounterClockwise = true;		//����� �� ������� ��� ������ ������� ��������� �������

	res = device->CreateRasterizerState(&rastDesc, &rastState);
	ZCHECK(res);

	game->texLoader->LoadTextureFromFile(textureName, texture, texSRV, true, false);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MaxLOD = INT_MAX;

	res = device->CreateSamplerState(&samplerDesc, &samplerState);
	ZCHECK(res);

	return 0;
}

HRESULT LightObjComponent::CreateShaders(ID3D11Device* device)
{
	HRESULT res;

	ID3DBlob* errorVertexCode;

	res = D3DCompileFromFile(L"TexLightShader.fx",
		nullptr,
		nullptr,
		"VSMain",												//���������� ���������� ������� (����� ����� � fx �����)
		"vs_5_0",												//��� �������_������ (vs - vertex shaider)
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,		//���������� ������ ��� ���������� �������� (| - or / ���)
		0,														//����� ��� ���������� ��������
		&vertexBC,												//���������������� ��������� ������ ���������� ���� (���� ��� ������)
		&errorVertexCode);										//��������� �� ������, ���� ��� �� ������� ��������������

	//�������� errorVertexCode �� ������� ������
	if (FAILED(res)) {
		if (errorVertexCode)
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			std::cout << "Missing Shader File" << std::endl;//
		}
		return 0;
	}

	//���������� ������
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"TexLightShader.fx",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelBC,
		&errorPixelCode);

	if (FAILED(res)) {
		if (errorPixelCode) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			std::cout << "Missing Shader File" << std::endl;//
		}
		return 0;
	}

	res = device->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader);
	ZCHECK(res);

	res = device->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader);
	ZCHECK(res);

	return 0;
}

HRESULT LightObjComponent::CreateLayout(ID3D11Device* device)
{
	HRESULT res;
	//-----------------------------------------------------------------------------
	//CREATE INPUT LAYOUT FOR IA STAGE
	//-----------------------------------------------------------------------------

	D3D11_INPUT_ELEMENT_DESC inputElements[] = 
	{
		D3D11_INPUT_ELEMENT_DESC { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC { "TEXTCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//�������� ������� ������
	res = device->CreateInputLayout(
		inputElements,					
		3,								
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	ZCHECK(res);

	return 0;
}

HRESULT LightObjComponent::CreateBufers(ID3D11Device* device)
{
	HRESULT res;

	//-----------------------------------------------------------------------------
	//CREATE VERTEX AND INDEX (OPTIONAL) BUFFERS
	//-----------------------------------------------------------------------------

	//���������� �����
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;									// ��, ��� ����� ����� ����������� ��������� �����
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							//���� ����� ����� ���� ��������
	vertexBufDesc.CPUAccessFlags = 0;											//0 -���� �� ����� ������ � ������ � ���
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vector4) * std::size(triangleObjPoints);	//������ ������ � ������

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = triangleObjPoints.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	res = device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);
	ZCHECK(res);

	//��������� �����
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(pointIndeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = pointIndeces.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	res = device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);
	ZCHECK(res);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstantData);

	res = device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);
	ZCHECK(res);

	//D3D11_BUFFER_DESC lightConstBufDesc = {};
	//lightConstBufDesc.Usage = D3D11_USAGE_DEFAULT;
	//lightConstBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//lightConstBufDesc.CPUAccessFlags = 0;
	//lightConstBufDesc.MiscFlags = 0;
	//lightConstBufDesc.StructureByteStride = 0;
	//lightConstBufDesc.ByteWidth = sizeof(LightData);
	//
	//res = device->CreateBuffer(&lightConstBufDesc, nullptr, &lightBuffer);
	//ZCHECK(res);

	return 0;
}

void LightObjComponent::Update(float deltaTime)//4
{										
	auto data = ConstantData{};
	data.World = Matrix::CreateTranslation(objectPosition);
	auto worldViewProj = Matrix::CreateTranslation(objectPosition) * gameCamera->viewMatrix * gameCamera->projectionMatrix;//������ �� ������� ��������� ��������� ������� � ���������� �� ������� ������ (������ ����������� ����� ����)
	worldViewProj = worldViewProj.Transpose();
	data.WorldViewProj = worldViewProj;
	data.Direction = Vector4(light_x, 2.0f, light_z, 1.0f);
	data.KaSpecPowKsX = Vector4(0.5f, 0.5f, 0.2f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE res = {};
	context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);					//������ � ����������� �����
	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &data, sizeof(ConstantData));										//�������� ������
	context->Unmap(constantBuffer, 0);
}

void LightObjComponent::Draw(ID3D11DeviceContext* context)
{
	UINT strides = 48;
	UINT offsets = 0;

	context->IASetInputLayout(layout);											//����������� ������� ������ � ���������� ���������
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//��������� ������� ��������� ������ � ������ (������ ������������)
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);			//���������� ��������� �����
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);		//���������� ��������� �����

	context->VSSetConstantBuffers(0, 1, &constantBuffer);						//���������� ����������� ����� (�������� �� ���������)
	context->PSSetConstantBuffers(0, 1, &constantBuffer);
	//context->PSSetConstantBuffers(1, 1, &lightBuffer);//

	context->PSSetShaderResources(0, 1, &texSRV);								//���������� SRV
	context->PSSetSamplers(0, 1, &samplerState);								//���������� �������

	//-----------------------------------------------------------------------------
	//SET VERTEX AND PIXEL SHADERS
	//-----------------------------------------------------------------------------
	context->VSSetShader(vertexShader, nullptr, 0);			//��������� ����������� �������
	context->PSSetShader(pixelShader, nullptr, 0);			//���������� ���������� ������

	context->RSSetState(rastState);

	context->DrawIndexed(36, 0, 0);//������ ��������������� ����� ��� ������
}

void LightObjComponent::DestroyResources()
{
	if (texSRV) texSRV->Release();
	if (rastState) rastState->Release();
	if (layout) layout->Release();
	if (vertexShader) vertexShader->Release();
	if (vertexBC) vertexBC->Release();
	if (pixelShader) pixelShader->Release();
	if (pixelBC) pixelBC->Release();
	if (vertexBuffer) vertexBuffer->Release();
	if (indexBuffer) indexBuffer->Release();
	if (samplerState) samplerState->Release();
	if (constantBuffer) constantBuffer->Release();	
}