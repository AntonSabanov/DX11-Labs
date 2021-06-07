#include "TextureObjComponent.h"

#include "pch.h"
#include "SimpleMath.h"

#include "Game.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;


TextureObjComponent::TextureObjComponent(Game* inGame, ID3D11Device* device, ID3D11DeviceContext* context, Vector3 startPosition, std::vector<Vector4> points, std::vector<int> indeces, LPCWSTR inFileName, LPCWSTR inTexName, Camera* camera)
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
	TextureObjComponent::context = context;
	gameCamera = camera;
	objectPosition = startPosition;//Vector3::Zero + Vector3(0, 1, 0);						//установка позиции объекта

	objFileName = inFileName;
	textureName = inTexName;

	Initialize(device, context);						//сразу инициализируем объект
}

HRESULT TextureObjComponent::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
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
	rastDesc.FillMode = D3D11_FILL_SOLID;		//принцып рисования объектов (SOLID - запоненный, WIREFRAME - только сетка)
	rastDesc.FrontCounterClockwise = true;		//обход по часовой или против часовой ститается фронтом
	
	res = device->CreateRasterizerState(&rastDesc, &rastState);
	ZCHECK(res);

	game->texLoader->LoadTextureFromFile(textureName, texture, texSRV, true, false); //грузим нашу текстуру

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

	res = device->CreateSamplerState(&samplerDesc, &samplerState); //создаем семплер стейт для текстуры
	ZCHECK(res);

	return 0;
}

HRESULT TextureObjComponent::CreateShaders(ID3D11Device* device)
{
	HRESULT res;

	ID3DBlob* errorVertexCode;

	res = D3DCompileFromFile(L"TextureShader.fx",
		nullptr,
		nullptr,
		"VSMain",												//компиляция вершинного шейдера (точка входа в fx файле)
		"vs_5_0",												//тип шейдера_версия (vs - vertex shaider)
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,		//комбинация флагов для компиляции шейдеров (| - or / или)
		0,														//флаги для компиляции эффектов
		&vertexBC,												//скомпилированный вершинный шейдер помещается сюда (байт код шедера)
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
	res = D3DCompileFromFile(L"TextureShader.fx",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //D3DCOMPILE_PACK_MATRIX_ROW_MAJOR
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

HRESULT TextureObjComponent::CreateLayout(ID3D11Device* device)
{
	HRESULT res;
	//-----------------------------------------------------------------------------
	//CREATE INPUT LAYOUT FOR IA STAGE
	//-----------------------------------------------------------------------------

	D3D11_INPUT_ELEMENT_DESC inputElements[] =
	{
		D3D11_INPUT_ELEMENT_DESC{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//создание шаблона вершин
	res = device->CreateInputLayout(
		inputElements,					//массив дескрипторов
		2,								//2 элемента в массиве дескрипторов
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	ZCHECK(res);

	return 0;
}

HRESULT TextureObjComponent::CreateBufers(ID3D11Device* device)
{
	HRESULT res;

	//-----------------------------------------------------------------------------
	//CREATE VERTEX AND INDEX (OPTIONAL) BUFFERS
	//-----------------------------------------------------------------------------
	
	//вертексный буфер
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;									// то, как часто будет обновляться вершинный буфер
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							//куда буфер может быть привязан
	vertexBufDesc.CPUAccessFlags = 0;											//0 -если не хотим чтения и записи с цпу
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
	constBufDesc.ByteWidth = sizeof(Matrix);

	res = device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);
	ZCHECK(res);

	return 0;
}

void TextureObjComponent::Update(float deltaTime)//4
{
	auto wvp = Matrix::CreateTranslation(objectPosition) * gameCamera->viewMatrix * gameCamera->projectionMatrix;//исходя из позиции создается транслешн матрица и умножается на матрицы камеры (камера обновляется перед этим)
	wvp = wvp.Transpose();

	D3D11_MAPPED_SUBRESOURCE res = {};
	context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);					//запись в константный буфер

	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &wvp, sizeof(Matrix));												//копируем данные из матрицы

	context->Unmap(constantBuffer, 0);													//подтверждение изменений в буфере
}

void TextureObjComponent::Draw(ID3D11DeviceContext* context)
{
	UINT strides = 32;
	UINT offsets = 0;

	context->IASetInputLayout(layout);											//подключение шаблона вершин к устройству рисования
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//установка способа отрисовки вершин в буфере (рисуем треугольники)
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);			//выставляем индексный буфер
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);		//выставляем вершинный буфер

	context->VSSetConstantBuffers(0, 1, &constantBuffer);						//выставляем константный буфер (отвечает за трансформ)

	context->PSSetShaderResources(0, 1, &texSRV);								//подключаем SRV
	context->PSSetSamplers(0, 1, &samplerState);								//подключаем семплер

	//-----------------------------------------------------------------------------
	//SET VERTEX AND PIXEL SHADERS
	//-----------------------------------------------------------------------------
	context->VSSetShader(vertexShader, nullptr, 0);			//установка вертексного шейдера
	context->PSSetShader(pixelShader, nullptr, 0);			//выставляем пиксельный шейдер

	context->RSSetState(rastState);

	context->DrawIndexed(36, 0, 0);//рисуем индексированные точки для кубика
}

void TextureObjComponent::DestroyResources()
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
	if (rastState) rastState->Release();
	if (samplerState) samplerState->Release();
	if (constantBuffer) constantBuffer->Release();
}


