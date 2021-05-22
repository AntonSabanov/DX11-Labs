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
	//	"VSMain", //���������� ���������� ������� (����� ����� � fx �����)
	//	"vs_5_0", //��� �������_������ (vs - vertex shaider)
	//	D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //���������� ������ ��� ���������� �������� (| - or / ���)
	//	0, //����� ��� ���������� ��������
	//	&vertexBC, //���������������� ��������� ������ ���������� ���� (���� ��� ������)
	//	&errorVertexCode); //��������� �� ������, ���� ��� �� ������� ��������������
	//
	////�������� errorVertexCode �� ������� ������
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
	////���������� ������
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
	////������ ������ ������ (������ ������) - ����� ������ ��������� �������� �������, ������� �� ���������� ������������, ����� ������ � � ����� ������� �������� � ��������� ������ (�������?)
	//D3D11_INPUT_ELEMENT_DESC inputElements[] = {
	//	D3D11_INPUT_ELEMENT_DESC {
	//		"POSITION", //������������� ��� ���� � �������
	//		0, //������������� ������ ���� (���� ���� ��������� ����� � ���������� ������������� ������)
	//		DXGI_FORMAT_R32G32B32A32_FLOAT, //������ (��� float4)
	//		0, //�������� ���� (0-15) ������ ������������� ���������� ������
	//		0, //����� ������ ������ � ������ ������ (����� � ������ ��� ����� �������� ������ ���������� ������ � �������� ����� �������) ������������, ���� � ���� ��������� ������ �������� ��������� ���������
	//		D3D11_INPUT_PER_VERTEX_DATA, //����� ��������� ����� (�� �����)
	//		0}, //InstanceDataStepRate (�� �����)
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
	////�������� ������� ������
	//ID3D11InputLayout* layout;
	//res = device->CreateInputLayout(
	//	inputElements, //������ ������������
	//	2, //2 �������� � ������� ������������
	//	vertexBC->GetBufferPointer(),
	//	vertexBC->GetBufferSize(),
	//	&layout);
	//ZCHECK(res);

	CreateBufers(device);
	//-----------------------------------------------------------------------------
	//CREATE SET OF POINTS (SOMETHINNG TO DRAW)
	//-----------------------------------------------------------------------------
	//DirectX::XMFLOAT4 points[8] = {
	//	XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
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
	//vertexBufDesc.ByteWidth = sizeof(XMFLOAT4) * std::size(points);//������ ������ � ������
	//vertexBufDesc.Usage = D3D11_USAGE_DEFAULT; // ��, ��� ����� ����� ����������� ��������� �����
	//vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //���� ����� ����� ���� ��������
	//vertexBufDesc.CPUAccessFlags = 0; //0 -���� �� ����� ������ � ������ � ���
	//vertexBufDesc.MiscFlags = 0;
	//vertexBufDesc.StructureByteStride = 0;
	//
	//D3D11_SUBRESOURCE_DATA vertexData = {};
	//vertexData.pSysMem = points;
	//vertexData.SysMemPitch = 0;
	//vertexData.SysMemSlicePitch = 0;
	//
	//ID3D11Buffer* vb;//���� ����� ������� ������� ��������� �����
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
	rastDesc.CullMode = D3D11_CULL_NONE;// �� ������ �������� ����������� �������� ������������ (none - �� ����� �����������, backface - ����������� ������)
	rastDesc.FillMode = D3D11_FILL_SOLID; //������� ��������� �������� (SOLID - ����������, WIREFRAME - ������ �����)
	//�������������� ��������� 2:10:00
	/*rastDesc.FrontCounterClockwise = false; //����� �� ������� ��� ������ ������� ��������� �������
	rastDesc.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;//��� ������� ������� �������
	rastDesc.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;//������������ �������� depth �����
	rastDesc.SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rastDesc.DepthClipEnable = true;//��������� ����, ��� ������ ������� ���������
	rastDesc.ScissorEnable = false;
	rastDesc.MultisampleEnable = false;//����� �� �������� �����������
	rastDesc.AntialiasedLineEnable = false;*/ //����������� ��� �����

	//ID3D11RasterizerState* rastState;
	res = device->CreateRasterizerState(&rastDesc, &rastState);
	ZCHECK(res);

	//context->RSSetState(rastState);
	//
	//// ������� � ����������� �������
	//D3D11_VIEWPORT viewport = {};//��������� ��������� ��� rastState
	//viewport.Width = static_cast<float>(game->appDisplay->screenWidth);
	//viewport.Height = static_cast<float>(game->appDisplay->screenHeight);
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.MinDepth = 0;
	//viewport.MaxDepth = 1.0f;
	//
	//context->RSSetViewports(1, &viewport);// ���������� ������� � ��������� ����������
	//
	////-----------------------------------------------------------------------------
	////SETUP BACKBUFER FOR OUTPUT
	////-----------------------------------------------------------------------------
	////����� ��� ���� ���������������?
	//context->OMSetRenderTargets(1, &(game->rtv), nullptr);// ���������� ������ ������� ������ � ��������� ���������� (���� ����� FLIP ������, �� ���� �������� �� ������ �����)
	return 0;
}

HRESULT TriangleComponent::CreateShaders(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	HRESULT res;

	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"MiniTri.fx",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain", //���������� ���������� ������� (����� ����� � fx �����)
		"vs_5_0", //��� �������_������ (vs - vertex shaider)
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //���������� ������ ��� ���������� �������� (| - or / ���)
		0, //����� ��� ���������� ��������
		&vertexBC, //���������������� ��������� ������ ���������� ���� (���� ��� ������)
		&errorVertexCode); //��������� �� ������, ���� ��� �� ������� ��������������

	//�������� errorVertexCode �� ������� ������
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

	//���������� ������
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
	//DirectX::XMFLOAT4 points[8] = {//������ ����� � ��������� � �������
	//	XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
	//	XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//	XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//};

	//int indeces[] = { 0,1,2, 1,0,3 };//������ ��������
	int indeces[] = {0, 1, 2};//������ ��������

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.ByteWidth = sizeof(XMFLOAT4) * std::size(trianglePoints);//������ ������ � ������
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT; // ��, ��� ����� ����� ����������� ��������� �����
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //���� ����� ����� ���� ��������
	vertexBufDesc.CPUAccessFlags = 0; //0 -���� �� ����� ������ � ������ � ���
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = trianglePoints.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//ID3D11Buffer* vb;//���� ����� ������� ������� ��������� �����
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
	//������ ������ ������ (������ ������) - ����� ������ ��������� �������� �������, ������� �� ���������� ������������, ����� ������ � � ����� ������� �������� � ��������� ������ (�������?)
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION", //������������� ��� ���� � �������
			0, //������������� ������ ���� (���� ���� ��������� ����� � ���������� ������������� ������)
			DXGI_FORMAT_R32G32B32A32_FLOAT, //������ (��� float4)
			0, //�������� ���� (0-15) ������ ������������� ���������� ������
			0, //����� ������ ������ � ������ ������ (����� � ������ ��� ����� �������� ������ ���������� ������ � �������� ����� �������) ������������, ���� � ���� ��������� ������ �������� ��������� ���������
			D3D11_INPUT_PER_VERTEX_DATA, //����� ��������� ����� (�� �����)
			0}, //InstanceDataStepRate (�� �����)
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	//�������� ������� ������
	//ID3D11InputLayout* layout;
	res = device->CreateInputLayout(
		inputElements, //������ ������������
		2, //2 �������� � ������� ������������
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

	//��������� ���������
	//int indeces[] = { 0, 1, 2 };
	context->IASetInputLayout(layout);//����������� ������� ������ � ���������� ���������
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//��������� ������� ��������� ������ � ������ (������ ������������)
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);//���������� ��������� �����
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);//���������� ��������� �����

	//-----------------------------------------------------------------------------
	//SET VERTEX AND PIXEL SHADERS
	//-----------------------------------------------------------------------------
	context->VSSetShader(vertexShader, nullptr, 0);//��������� ����������� �������
	context->PSSetShader(pixelShader, nullptr, 0);//���������� ���������� ������

	context->RSSetState(rastState);

	//annotation->BeginEvent(L"BeginDraw");
	context->DrawIndexed(3, 0, 0);
	//annotation->EndEvent();

	//������� � ����������� �������
	//D3D11_VIEWPORT viewport = {};//��������� ��������� ��� rastState
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
	//context->DrawIndexed(6, 0, 0);//��������� ���������� ������ ������ � ���������
	//game->annotation->EndEvent();
}

void TriangleComponent::DestroyResources() 
{
	if (layout) layout->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}