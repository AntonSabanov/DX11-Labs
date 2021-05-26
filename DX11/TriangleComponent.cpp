#include "pch.h"
#include "TriangleComponent.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

TriangleComponent::TriangleComponent(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vector4> points, Camera* camera)
{
	//game = gameObj;
	for (size_t i = 0; i < points.size(); ++i) 
	{
		triangleObjPoints.emplace_back(points[i]);
	}
	TriangleComponent::context = context;
	gameCamera = camera;
	objectPosition = Vector3::Zero;//установка позиции объекта

	Initialize(device, context);//сразу инициализируем объект
}

HRESULT TriangleComponent::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
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

	res = device->CreateRasterizerState(&rastDesc, &rastState);
	ZCHECK(res);

	
	return 0;
}

HRESULT TriangleComponent::CreateShaders(ID3D11Device* device)
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
			std::cout << "Missing Shader File" << std::endl;//
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

HRESULT TriangleComponent::CreateLayout(ID3D11Device* device)
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
	res = device->CreateInputLayout(
		inputElements, //массив дескрипторов
		2, //2 элемента в массиве дескрипторов
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	ZCHECK(res);

	return 0;
}

HRESULT TriangleComponent::CreateBufers(ID3D11Device* device)
{
	HRESULT res;

	//-----------------------------------------------------------------------------
	//CREATE VERTEX AND INDEX (OPTIONAL) BUFFERS
	//-----------------------------------------------------------------------------
	int indeces[] = { 0,1,2, 1,0,3 };//массив индексов
	//int indeces[] = {0, 1, 2};//массив индексов

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.ByteWidth = sizeof(Vector4) * std::size(triangleObjPoints);//размер буфера в байтах
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT; // то, как часто будет обновляться вершинный буфер
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //куда буфер может быть привязан
	vertexBufDesc.CPUAccessFlags = 0; //0 -если не хотим чтения и записи с цпу
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = triangleObjPoints.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

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

	res = device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);
	ZCHECK(res);

	//создаем константный буфер
	D3D11_BUFFER_DESC constBufDesc = {};
	indexBufDesc.ByteWidth = sizeof(Matrix);
	indexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	indexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;

	res = device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);
	ZCHECK(res);

	return 0;
}

void TriangleComponent::Update(float deltaTime)//4
{
	auto wvp = Matrix::CreateTranslation(objectPosition) * gameCamera->viewMatrix * gameCamera->projectionMatrix;//исходя из позиции создается транслешн матрица и умножается на матрицы камеры (камера обновляется перед этим)

	D3D11_MAPPED_SUBRESOURCE res = {};
	context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);					//запись в константный буфер

	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &wvp, sizeof(Matrix));												//копируем данные из матрицы

	context->Unmap(constantBuffer, 0);													//подтверждение изменений в буфере
}

void TriangleComponent::Draw(ID3D11DeviceContext* context)
{
	UINT strides = 32;
	UINT offsets = 0;

	context->IASetInputLayout(layout);//подключение шаблона вершин к устройству рисования
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//установка способа отрисовки вершин в буфере (рисуем треугольники)
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);//выставляем индексный буфер
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);//выставляем вершинный буфер

	context->VSSetConstantBuffers(0, 1, &constantBuffer);//выставляем константный буфер

	//-----------------------------------------------------------------------------
	//SET VERTEX AND PIXEL SHADERS
	//-----------------------------------------------------------------------------
	context->VSSetShader(vertexShader, nullptr, 0);//установка вертексного шейдера
	context->PSSetShader(pixelShader, nullptr, 0);//выставляем пиксельный шейдер

	context->RSSetState(rastState);

	//annotation->BeginEvent(L"BeginDraw");
	 
	//context->DrawIndexed(3, 0, 0);
	context->DrawIndexed(6, 0, 0);//рисуем индексированные точки
	//context->DrawAuto();

	//annotation->EndEvent();
}

void TriangleComponent::DestroyResources() 
{
	if (layout) layout->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}

