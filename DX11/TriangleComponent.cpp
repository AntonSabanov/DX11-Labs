#include "pch.h"
#include "TriangleComponent.h"

using namespace DirectX;

TriangleComponent::TriangleComponent(Microsoft::WRL::ComPtr<ID3D11Device> device, ID3D11DeviceContext* context, std::vector<DirectX::XMFLOAT4> points)
{
	//game = gameObj;
	for (size_t i = 0; i < points.size(); ++i) 
	{
		trianglePoints.emplace_back(points[i]);
	}
	Initialize(device, context);
}

HRESULT TriangleComponent::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, ID3D11DeviceContext* context)
{
	HRESULT res;

	CreateShaders(device);
	//ID3DBlob* errorVertexCode;
	//res = D3DCompileFromFile(L"MiniTri.fx",
	//	nullptr /*macros*/,
	//	nullptr /*include*/,
	//	"VSMain", //компиляция вершинного шейдера (точка входа в fx файле)
	//	"vs_5_0", //тип шейдера_версия (vs - vertex shaider)
	//	D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //комбинация флагов для компиляции шейдеров (| - or / или)
	//	0, //флаги для компиляции эффектов
	//	&vertexBC, //скомпилированный вершинный шейдер помещается сюда (байт код шедера)
	//	&errorVertexCode); //сообщение об ошибке, если код не удалось скомпилировать
	//
	////проверка errorVertexCode на наличие ошибки
	//if (FAILED(res)) {
	//	// If the shader failed to compile it should have written something to the error message.
	//	if (errorVertexCode) {
	//		char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());
	//
	//		std::cout << compileErrors << std::endl;
	//	}
	//	// If there was  nothing in the error message then it simply could not find the shader file itself.
	//	else
	//	{
	//		//MessageBox(game->appDisplay->hWnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
	//	}
	//
	//	return 0;
	//}
	//
	////пиксельный шейдер
	//ID3DBlob* errorPixelCode;
	//res = D3DCompileFromFile(L"MiniTri.fx",
	//	nullptr /*macros*/,
	//	nullptr /*include*/,
	//	"PSMain",
	//	"ps_5_0",
	//	D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	//	0,
	//	&pixelBC,
	//	&errorPixelCode);
	//
	//if (FAILED(res)) {
	//	// If the shader failed to compile it should have written something to the error message.
	//	if (errorPixelCode) {
	//		char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());
	//
	//		std::cout << compileErrors << std::endl;
	//	}
	//	// If there was  nothing in the error message then it simply could not find the shader file itself.
	//	else
	//	{
	//		//MessageBox(game->appDisplay->hWnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
	//	}
	//
	//	return 0;
	//}
	//
	//res = device->CreateVertexShader(
	//	vertexBC->GetBufferPointer(),
	//	vertexBC->GetBufferSize(),
	//	nullptr, &vertexShader);
	//ZCHECK(res);
	//
	//res = device->CreatePixelShader(
	//	pixelBC->GetBufferPointer(),
	//	pixelBC->GetBufferSize(),
	//	nullptr, &pixelShader);
	//ZCHECK(res);
	//
	
	CreateLayout(device);
	////-----------------------------------------------------------------------------
	////CREATE INPUT LAYOUT FOR IA STAGE
	////-----------------------------------------------------------------------------
	////формат данных вершин (шаблон вершин) - какие именно параметры содержат вершины, которые мы собираемся использовать, какие данные и в каком порядке хранятся в вершинном буфере (шейдере?)
	//D3D11_INPUT_ELEMENT_DESC inputElements[] = {
	//	D3D11_INPUT_ELEMENT_DESC {
	//		"POSITION", //семантическое имя поля в шейдере
	//		0, //семантический индекс поля (если есть несколько полей с одинаковым семантическим именем)
	//		DXGI_FORMAT_R32G32B32A32_FLOAT, //размер (для float4)
	//		0, //входящий слот (0-15) индекс подключенного вершинного буфера
	//		0, //адрес начала данных в буфере вершин (сдвиг в байтах для этого элемента внутри вершинного буфера в пределах одной вершины) используется, если в одно вершинном буфере хранится несколько элементов
	//		D3D11_INPUT_PER_VERTEX_DATA, //класс входящего слота (не важно)
	//		0}, //InstanceDataStepRate (не важно)
	//	D3D11_INPUT_ELEMENT_DESC {
	//		"COLOR",
	//		0,
	//		DXGI_FORMAT_R32G32B32A32_FLOAT,
	//		0,
	//		D3D11_APPEND_ALIGNED_ELEMENT,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0}
	//};
	//
	////создание шаблона вершин
	//ID3D11InputLayout* layout;
	//res = device->CreateInputLayout(
	//	inputElements, //массив дескрипторов
	//	2, //2 элемента в массиве дескрипторов
	//	vertexBC->GetBufferPointer(),
	//	vertexBC->GetBufferSize(),
	//	&layout);
	//ZCHECK(res);

	CreateBufers(device);
	//-----------------------------------------------------------------------------
	//CREATE SET OF POINTS (SOMETHINNG TO DRAW)
	//-----------------------------------------------------------------------------
	//DirectX::XMFLOAT4 points[8] = {
	//	XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет
	//	XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//	XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//};
	//
	////-----------------------------------------------------------------------------
	////CREATE VERTEX AND INDEX (OPTIONAL) BUFFERS
	////-----------------------------------------------------------------------------
	//int indeces[] = { 0,1,2, 1,0,3 };
	//
	//D3D11_BUFFER_DESC vertexBufDesc = {};
	//vertexBufDesc.ByteWidth = sizeof(XMFLOAT4) * std::size(points);//размер буфера в байтах
	//vertexBufDesc.Usage = D3D11_USAGE_DEFAULT; // то, как часто будет обновляться вершинный буфер
	//vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //куда буфер может быть привязан
	//vertexBufDesc.CPUAccessFlags = 0; //0 -если не хотим чтения и записи с цпу
	//vertexBufDesc.MiscFlags = 0;
	//vertexBufDesc.StructureByteStride = 0;
	//
	//D3D11_SUBRESOURCE_DATA vertexData = {};
	//vertexData.pSysMem = points;
	//vertexData.SysMemPitch = 0;
	//vertexData.SysMemSlicePitch = 0;
	//
	//ID3D11Buffer* vb;//сюда будет записан готовый вершинный буфер
	//res = device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);
	//ZCHECK(res);
	//
	//D3D11_BUFFER_DESC indexBufDesc = {};
	//indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);
	//indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufDesc.CPUAccessFlags = 0;
	//indexBufDesc.MiscFlags = 0;
	//indexBufDesc.StructureByteStride = 0;
	//
	//D3D11_SUBRESOURCE_DATA indexData = {};
	//indexData.pSysMem = indeces;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;
	//
	//ID3D11Buffer* ib;
	//res = device->CreateBuffer(&indexBufDesc, &indexData, &ib);
	//ZCHECK(res);
	
	//-----------------------------------------------------------------------------
	//SETUP RASTERIZER STAGE AND VIEWPORT
	//-----------------------------------------------------------------------------
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;// по какому признаку одбрасываем ненужные треугольники (none - не будем отбрасывать, backface - отбрасывать задние)
	rastDesc.FillMode = D3D11_FILL_SOLID; //принцып рисования объектов (SOLID - запоненный, WIREFRAME - только сетка)
	//дополнительные параметры 2:10:00
	/*rastDesc.FrontCounterClockwise = false; //обход по часовой или против часовой ститается фронтом
	rastDesc.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;//при расчете глубины пикселя
	rastDesc.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;//максимальное значение depth биаса
	rastDesc.SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rastDesc.DepthClipEnable = true;//отсичение того, что дальше области видимости
	rastDesc.ScissorEnable = false;
	rastDesc.MultisampleEnable = false;//будет ли работать сглаживание
	rastDesc.AntialiasedLineEnable = false;*/ //сглаживание для линий

	//ID3D11RasterizerState* rastState;
	res = device->CreateRasterizerState(&rastDesc, &rastState);
	ZCHECK(res);

	//context->RSSetState(rastState);
	//
	//// создаем и настраиваем вьюпорт
	//D3D11_VIEWPORT viewport = {};//установка вьюпортов для rastState
	//viewport.Width = static_cast<float>(game->appDisplay->screenWidth);
	//viewport.Height = static_cast<float>(game->appDisplay->screenHeight);
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.MinDepth = 0;
	//viewport.MaxDepth = 1.0f;
	//
	//context->RSSetViewports(1, &viewport);// Подключаем вьюпорт к контексту устройства
	//
	////-----------------------------------------------------------------------------
	////SETUP BACKBUFER FOR OUTPUT
	////-----------------------------------------------------------------------------
	////зачем два раза устанавливается?
	//context->OMSetRenderTargets(1, &(game->rtv), nullptr);// Подключаем объект заднего буфера к контексту устройства (если стоит FLIP эффект, то надо вызывать на каждом кадре)
	return 0;
}

HRESULT TriangleComponent::CreateShaders(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	HRESULT res;

	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"MiniTri.fx",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain", //компиляция вершинного шейдера (точка входа в fx файле)
		"vs_5_0", //тип шейдера_версия (vs - vertex shaider)
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //комбинация флагов для компиляции шейдеров (| - or / или)
		0, //флаги для компиляции эффектов
		&vertexBC, //скомпилированный вершинный шейдер помещается сюда (байт код шедера)
		&errorVertexCode); //сообщение об ошибке, если код не удалось скомпилировать

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
			//MessageBox(game->appDisplay->hWnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
		}

		return 0;
	}

	//пиксельный шейдер
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"MiniTri.fx",
		nullptr /*macros*/,
		nullptr /*include*/,
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
			//MessageBox(game->appDisplay->hWnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
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

HRESULT TriangleComponent::CreateBufers(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	HRESULT res;

	//-----------------------------------------------------------------------------
	//CREATE VERTEX AND INDEX (OPTIONAL) BUFFERS
	//-----------------------------------------------------------------------------
	//DirectX::XMFLOAT4 points[8] = {//массив точек с позициями и цветами
	//	XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет
	//	XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//	XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//};

	//int indeces[] = { 0,1,2, 1,0,3 };//массив индексов
	int indeces[] = {0, 1, 2};//массив индексов

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.ByteWidth = sizeof(XMFLOAT4) * std::size(trianglePoints);//размер буфера в байтах
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT; // то, как часто будет обновляться вершинный буфер
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //куда буфер может быть привязан
	vertexBufDesc.CPUAccessFlags = 0; //0 -если не хотим чтения и записи с цпу
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = trianglePoints.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//ID3D11Buffer* vb;//сюда будет записан готовый вершинный буфер
	res = device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);
	ZCHECK(res);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//ID3D11Buffer* ib;
	res = device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);
	ZCHECK(res);

	return 0;
}

HRESULT TriangleComponent::CreateLayout(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	HRESULT res;
	//-----------------------------------------------------------------------------
	//CREATE INPUT LAYOUT FOR IA STAGE
	//-----------------------------------------------------------------------------
	//формат данных вершин (шаблон вершин) - какие именно параметры содержат вершины, которые мы собираемся использовать, какие данные и в каком порядке хранятся в вершинном буфере (шейдере?)
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION", //семантическое имя поля в шейдере
			0, //семантический индекс поля (если есть несколько полей с одинаковым семантическим именем)
			DXGI_FORMAT_R32G32B32A32_FLOAT, //размер (для float4)
			0, //входящий слот (0-15) индекс подключенного вершинного буфера
			0, //адрес начала данных в буфере вершин (сдвиг в байтах для этого элемента внутри вершинного буфера в пределах одной вершины) используется, если в одно вершинном буфере хранится несколько элементов
			D3D11_INPUT_PER_VERTEX_DATA, //класс входящего слота (не важно)
			0}, //InstanceDataStepRate (не важно)
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
	//ID3D11InputLayout* layout;
	res = device->CreateInputLayout(
		inputElements, //массив дескрипторов
		2, //2 элемента в массиве дескрипторов
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	ZCHECK(res);

	return 0;
}

void TriangleComponent::Draw(ID3D11DeviceContext* context)
{
	UINT strides = 32;
	UINT offsets = 0;

	//настройка конвейера
	//int indeces[] = { 0, 1, 2 };
	context->IASetInputLayout(layout);//подключение шаблона вершин к устройству рисования
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//установка способа отрисовки вершин в буфере (рисуем треугольники)
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);//выставляем индексный буфер
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);//выставляем вершинный буфер

	//-----------------------------------------------------------------------------
	//SET VERTEX AND PIXEL SHADERS
	//-----------------------------------------------------------------------------
	context->VSSetShader(vertexShader, nullptr, 0);//установка вертексного шейдера
	context->PSSetShader(pixelShader, nullptr, 0);//выставляем пиксельный шейдер

	context->RSSetState(rastState);

	//annotation->BeginEvent(L"BeginDraw");
	context->DrawIndexed(3, 0, 0);
	//annotation->EndEvent();

	//создаем и настраиваем вьюпорт
	//D3D11_VIEWPORT viewport = {};//установка вьюпортов для rastState
	//viewport.Width = static_cast<float>(game->appDisplay->screenWidth);
	//viewport.Height = static_cast<float>(game->appDisplay->screenHeight);
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.MinDepth = 0;
	//viewport.MaxDepth = 1.0f;

	//context->RSSetViewports(1, &viewport);

	////-----------------------------------------------------------------------------
	////SETUP BACKBUFER FOR OUTPUT
	////-----------------------------------------------------------------------------
	//context->OMSetRenderTargets(1, &(game->rtv), nullptr);

	//game->annotation->BeginEvent(L"BeginDraw");
	//context->DrawIndexed(6, 0, 0);//отрисовка вершинного буфера вместе с индексным
	//game->annotation->EndEvent();
}

void TriangleComponent::DestroyResources() 
{
	if (layout) layout->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}