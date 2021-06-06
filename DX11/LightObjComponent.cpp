#include "LightObjComponent.h"
#include "pch.h"
#include "SimpleMath.h"
#include "Game.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

struct ConstantData
{
	Matrix WorldViewProj;
	Matrix World;
	Vector4 ViewerPos;
};

struct LightData
{
	Vector4 Direction;
	Vector4 Color;
	Vector4 KaSpecPowKsX;
};

LightObjComponent::LightObjComponent(Game* inGame, ID3D11Device* device, ID3D11DeviceContext* context, Vector3 startPosition, std::vector<Vector4> points, std::vector<int> indeces, LPCWSTR inFileName, LPCWSTR inTexName, Camera* camera)
{
	game = inGame;

	for (size_t i = 0; i < points.size(); ++i)
	{
		triangleObjPoints.emplace_back(points[i]);
	}
	for (size_t i = 0; i < indeces.size(); ++i)			//заполнение массива индексов
	{
		pointIndeces.emplace_back(indeces[i]);
	}
	LightObjComponent::context = context;
	gameCamera = camera;
	objectPosition = startPosition;						//установка позиции объекта

	objFileName = inFileName;
	textureName = inTexName;

	Initialize(device, context);						//сразу инициализируем объект
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
	rastDesc.CullMode = D3D11_CULL_BACK;		// по какому признаку одбрасываем ненужные треугольники (NONE - не будем отбрасывать, BACK - отбрасывать задние,FRONT)
	rastDesc.FillMode = D3D11_FILL_SOLID;		//принцып рисовани€ объектов (SOLID - запоненный, WIREFRAME - только сетка)
	rastDesc.FrontCounterClockwise = true;		//обход по часовой или против часовой ститаетс€ фронтом

	res = device->CreateRasterizerState(&rastDesc, &rastState);
	ZCHECK(res);

	game->texLoader->LoadTextureFromFile(textureName, texture, texSRV, true, false); //грузим текстуру

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

	res = device->CreateSamplerState(&samplerDesc, &samplerState); //создаем семплер стейт дл€ текстуры
	ZCHECK(res);

	return 0;
}

HRESULT LightObjComponent::CreateShaders(ID3D11Device* device)
{
	HRESULT res;

	ID3DBlob* errorVertexCode;

	res = D3DCompileFromFile(L"TexLightShader.fx",//L"TextureShader.fx",
		nullptr,
		nullptr,
		"VSMain",												//компил€ци€ вершинного шейдера (точка входа в fx файле)
		"vs_5_0",												//тип шейдера_верси€ (vs - vertex shaider)
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, //D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,		//комбинаци€ флагов дл€ компил€ции шейдеров (| - or / или)
		0,														//флаги дл€ компил€ции эффектов
		&vertexBC,												//скомпилированный вершинный шейдер помещаетс€ сюда (байт код шедера)
		&errorVertexCode);										//сообщение об ошибке, если код не удалось скомпилировать

	//проверка errorVertexCode на наличие ошибки
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

	//пиксельный шейдер
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"TexLightShader.fx",//L"TextureShader.fx",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, //D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //D3DCOMPILE_PACK_MATRIX_ROW_MAJOR
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

	//создание шаблона вершин
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

	//вертексный буфер
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;									// то, как часто будет обновл€тьс€ вершинный буфер
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							//куда буфер может быть прив€зан
	vertexBufDesc.CPUAccessFlags = 0;											//0 -если не хотим чтени€ и записи с цпу
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vector4) * std::size(triangleObjPoints);	//размер буфера в байтах

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = triangleObjPoints.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	res = device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);
	ZCHECK(res);

	//индексный буфер
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

	D3D11_BUFFER_DESC lightConstBufDesc = {};
	lightConstBufDesc.Usage = D3D11_USAGE_DEFAULT;
	lightConstBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightConstBufDesc.CPUAccessFlags = 0;
	lightConstBufDesc.MiscFlags = 0;
	lightConstBufDesc.StructureByteStride = 0;
	lightConstBufDesc.ByteWidth = sizeof(LightData);

	res = device->CreateBuffer(&lightConstBufDesc, nullptr, &lightBuffer);

	return 0;
}

void LightObjComponent::Update(float deltaTime)//4
{
	auto wvp = Matrix::CreateTranslation(objectPosition) * gameCamera->viewMatrix * gameCamera->projectionMatrix;//исход€ из позиции создаетс€ транслешн матрица и умножаетс€ на матрицы камеры (камера обновл€етс€ перед этим)
	//wvp = wvp.Transpose();

	D3D11_MAPPED_SUBRESOURCE res = {};
	context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);					//запись в константный буфер

	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &wvp, sizeof(Matrix));												//копируем данные из матрицы

	context->Unmap(constantBuffer, 0);													//подтверждение изменений в буфере
}

void LightObjComponent::Draw(ID3D11DeviceContext* context)
{
	UINT strides = 48;//
	UINT offsets = 0;

	context->IASetInputLayout(layout);											//подключение шаблона вершин к устройству рисовани€
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//установка способа отрисовки вершин в буфере (рисуем треугольники)
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);			//выставл€ем индексный буфер
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);		//выставл€ем вершинный буфер

	context->VSSetConstantBuffers(0, 1, &constantBuffer);						//выставл€ем константный буфер (отвечает за трансформ)
	context->PSSetConstantBuffers(1, 1, &lightBuffer);

	context->PSSetShaderResources(0, 1, &texSRV);								//подключаем SRV
	context->PSSetSamplers(0, 1, &samplerState);								//подключаем семплер

	//-----------------------------------------------------------------------------
	//SET VERTEX AND PIXEL SHADERS
	//-----------------------------------------------------------------------------
	context->VSSetShader(vertexShader, nullptr, 0);			//установка вертексного шейдера
	context->PSSetShader(pixelShader, nullptr, 0);			//выставл€ем пиксельный шейдер

	context->RSSetState(rastState);

	context->DrawIndexed(36, 0, 0);//рисуем индексированные точки дл€ кубика
}

void LightObjComponent::DestroyResources()
{
	if (layout) layout->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}