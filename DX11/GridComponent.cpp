#include "pch.h"
#include "GridComponent.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

GridComponent::GridComponent(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vector4> points, std::vector<int> indeces, Camera* camera)
{
	for (size_t i = 0; i < points.size(); ++i)//заполнение массива координат
	{
		objectPoints.emplace_back(points[i]);
	}
	for (size_t i = 0; i < indeces.size(); ++i)//заполнение массива индексов
	{
		pointIndeces.emplace_back(indeces[i]);
	}

	//worldMatrix = DirectX::SimpleMath::Matrix::Identity;//
	

	GridComponent::context = context;
	gameCamera = camera;
	objectPosition = Vector3::Zero;//установка позиции объекта

	worldMatrix = Matrix::CreateWorld(objectPosition, Vector3::Forward, Vector3::Up);

	Initialize(device, context);//сразу инициализируем объект
}

HRESULT GridComponent::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT res;

	CreateShaders(device);
	CreateLayout(device);
	CreateBufers(device);

	//-----------------------------------------------------------------------------
	//SETUP RASTERIZER STAGE AND VIEWPORT
	//-----------------------------------------------------------------------------
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;// по какому признаку одбрасываем ненужные треугольники (NONE - не будем отбрасывать, BACK - отбрасывать задние,FRONT)
	rastDesc.FillMode = D3D11_FILL_WIREFRAME; //принцып рисования объектов (SOLID - запоненный, WIREFRAME - только сетка)

	res = device->CreateRasterizerState(&rastDesc, &rastState);
	ZCHECK(res);

	return S_OK;
}

HRESULT GridComponent::CreateShaders(ID3D11Device* device)
{
	HRESULT res;

	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"MiniTri.fx",
		nullptr,
		nullptr,
		"VSMain", 
		"vs_5_0", 
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 
		0, 
		&vertexBC, 
		&errorVertexCode);

	//проверка errorVertexCode на наличие ошибки
	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File" << std::endl;//
		}

		return 0;
	}

	//пиксельный шейдер
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"MiniTri.fx",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelBC,
		&errorPixelCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
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

	return S_OK;
}

HRESULT GridComponent::CreateLayout(ID3D11Device* device)
{
	HRESULT res;
	//-----------------------------------------------------------------------------
	//CREATE INPUT LAYOUT FOR IA STAGE
	//-----------------------------------------------------------------------------
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT, 
			0, 
			0, 
			D3D11_INPUT_PER_VERTEX_DATA, 
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	//создание шаблона вершин
	res = device->CreateInputLayout(
		inputElements, 
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	ZCHECK(res);

	return S_OK;
}

HRESULT GridComponent::CreateBufers(ID3D11Device* device)
{
	HRESULT res;

	//-----------------------------------------------------------------------------
	//CREATE VERTEX AND INDEX (OPTIONAL) BUFFERS
	//-----------------------------------------------------------------------------

	//int indeces[] = { //массив индексов для кубика
	//	0,1,
	//	2,3,
	//	4,5,
	//	6,7,
	//	8,9};

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vertexBufDesc.CPUAccessFlags = 0; 
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vector4) * std::size(objectPoints);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = objectPoints.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	res = device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);
	ZCHECK(res);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.ByteWidth = sizeof(int) * std::size(pointIndeces);
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = pointIndeces.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	res = device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);
	ZCHECK(res);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.ByteWidth = sizeof(Matrix);
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;	

	res = device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);
	ZCHECK(res);

	return res;
}

void GridComponent::Draw(ID3D11DeviceContext* context)
{
	UINT strides = 32;
	UINT offsets = 0;

	context->IASetInputLayout(layout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);//рисуем линии
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);

	context->VSSetConstantBuffers(0, 1, &constantBuffer);

	//-----------------------------------------------------------------------------
	//SET VERTEX AND PIXEL SHADERS
	//-----------------------------------------------------------------------------
	context->VSSetShader(vertexShader, nullptr, 0);//установка вертексного шейдера
	context->PSSetShader(pixelShader, nullptr, 0);//выставляем пиксельный шейдер

	context->RSSetState(rastState);

	//context->DrawIndexed(36, 0, 0);//рисуем индексированные точки для кубика
	context->Draw(pointIndeces.size(), 0);
}

void GridComponent::Update(float deltaTime)
{
	auto wvp = Matrix::CreateTranslation(objectPosition) * gameCamera->viewMatrix * gameCamera->projectionMatrix;//исходя из позиции создается транслешн матрица и умножается на матрицы камеры (камера обновляется перед этим)
	wvp = wvp.Transpose();
	
	D3D11_MAPPED_SUBRESOURCE res = {};
	context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);					//запись в константный буфер

	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &wvp, sizeof(Matrix));												//копируем данные из матрицы

	context->Unmap(constantBuffer, 0);
}

void GridComponent::DestroyResources()
{
	if (layout) layout->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}