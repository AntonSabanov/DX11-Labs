#include "DoubleGame.h"
#include "SimpleMath.h"
#include "GridComponent.h"
#include "TextureObjComponent.h"
#include "LightObjComponent.h"
#include "Keys.h"

using namespace DirectX::SimpleMath;

DoubleGame::DoubleGame(std::vector<GameTransform*> games)
{
	for (size_t i = 0; i < games.size(); ++i)
	{
		gameWindows.emplace_back(games[i]);
	}
}

HRESULT DoubleGame::DoubleRun()
{
	PrepareRecources();
	for (size_t i = 0; i < gameWindows.size(); ++i)
	{
		gameWindows[i]->CreateLocalRecources();//������� ���������, �������� ��� � ��.
	}
	Initialize();

	MSG msg = {};

	bool isExitRequested = false;
	while (!isExitRequested)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			isExitRequested = true;
		}

		Draw();//update internal
	}

	//DestroyRecources();
	return 0;
}

HRESULT DoubleGame::PrepareRecources()//�������� ������� � ���������
{
	HRESULT res; 
	
	//DXGI_SWAP_CHAIN_DESC swapDesc1 = {};
	//swapDesc1.BufferCount = 2;													//� ��� 2 ������ ������(������ ��� � FLIP_DISCARD ������ ���� ��� ������� 2 ������)
	//swapDesc1.BufferDesc.Width = gameWindows[0]->appDisplay->screenWidth;						//������ ��������� ������	
	//swapDesc1.BufferDesc.Height = gameWindows[0]->appDisplay->screenHeight;						//������ ��������� ������
	//swapDesc1.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;					//������ ������� � ������
	//swapDesc1.BufferDesc.RefreshRate.Numerator = 60;								//������� ���������� ������
	//swapDesc1.BufferDesc.RefreshRate.Denominator = 1;							//� �������
	//swapDesc1.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//swapDesc1.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				//���������� ������ ��� ����
	//swapDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						//���������� ������ - ���������� � ���� ��� ��� �� ���������
	//swapDesc1.OutputWindow = gameWindows[0]->appDisplay->hWnd;									//�������� � ������ ���� WinApi
	//swapDesc1.Windowed = true;													//�� ������������� �����
	//swapDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;						//����������, ��� ���������� � ���������� ��� ������ ����� ������������
	//swapDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;					//�������� �� ����������� ������� ��� �������
	//swapDesc1.SampleDesc.Count = 1;												//�������� �� �������������� � ������� �� ��������� SwapEffect
	//swapDesc1.SampleDesc.Quality = 0;											//��. ����

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 }; //�������������� ����
	
	res = D3D11CreateDevice(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE, 
		nullptr, 
		D3D11_CREATE_DEVICE_DEBUG, 
		featureLevel,													//11_0
		1,
		D3D11_SDK_VERSION, 
		&device, 
		nullptr,														//��������� �� ������ �������������� ������� � ������� featureLefel
		&context);

	for (size_t i = 0; i < gameWindows.size(); ++i)
	{
		gameWindows[i]->device = device;
		gameWindows[i]->context = context;
	}
	//res = D3D11CreateDeviceAndSwapChain(
	//	nullptr,														//default adapter
	//	D3D_DRIVER_TYPE_HARDWARE,										//videocard driver
	//	nullptr,
	//	D3D11_CREATE_DEVICE_DEBUG,
	//	featureLevel,													//11_0
	//	1,
	//	D3D11_SDK_VERSION,
	//	&swapDesc1,														//��������� ��� ���������
	//	&swapChain,														//��� ��������
	//	&device,														//������
	//	nullptr,														//��������� �� ������ �������������� ������� � ������� featureLefel
	//	&context);														//�������� ����������
	//ZCHECK(res);	
												
	//swapDesc1.OutputWindow = appDisplay2->hWnd;
	//swapDesc1.BufferDesc.Width = appDisplay2->screenWidth;						
	//swapDesc1.BufferDesc.Height = appDisplay2->screenHeight;

	//������� ��� ���� ���� ����
	//res = device->QueryInterface(__uuidof(IDXGIDevice), (void**)&superDevice);
	//if (SUCCEEDED(res))
	//res = superDevice->GetAdapter(&adapter);
	//
	//if (SUCCEEDED(res))
	//{
	//	adapter->GetParent(IID_PPV_ARGS(&factory));
	//	res = factory->CreateSwapChain(device, &swapDesc1, &swapChain2);
	//	ZCHECK(res);
	//}
	//
	//////////////////////////////////////������� ������ �����
	//res = swapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);	//������� �����
	//ZCHECK(res);
	//res = swapChain2->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer2);	//������� �����
	//ZCHECK(res);
	//
	//res = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);			// ������������� ������ ����� ��� ������ ������
	//ZCHECK(res);
	//res = device->CreateRenderTargetView(backBuffer2, nullptr, &rtv2);			// ������������� ������ ����� ��� ������ ������
	//ZCHECK(res);
	//
	//targets[0] = rtv;
	//targets[1] = rtv2;
	//res = device->CreateRenderTargetView(backBuffer, nullptr, &targets);
	//ZCHECK(res);
	//res = device->CreateRenderTargetView(backBuffer2, nullptr, &targets);
	//ZCHECK(res);

	//D3D11_TEXTURE2D_DESC depthTexDesc = {};
	//depthTexDesc.ArraySize = 1;										//���� �������� � ��� ������
	//depthTexDesc.MipLevels = 1;										//������� �������
	//depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;//
	//depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	//depthTexDesc.CPUAccessFlags = 0;
	//depthTexDesc.MiscFlags = 0;
	//depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthTexDesc.Width = appDisplay->screenWidth;
	//depthTexDesc.Height = appDisplay->screenHeight;
	//depthTexDesc.SampleDesc = { 1, 0 };
	//
	//res = device->CreateTexture2D(&depthTexDesc, nullptr, &depthBuffer);
	//ZCHECK(res);
	//
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	//depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthStencilDesc.Flags = 0;
	//
	//res = device->CreateDepthStencilView(depthBuffer, &depthStencilDesc, &depthView);
	//ZCHECK(res);
	
	///////////////////////////////

	//swapChain->QueryInterface<IDXGISwapChain1>(&swapChain_1);//
	//swapChain2->QueryInterface<IDXGISwapChain1>(&swapChain_2);
	//
	//context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation);
	//
	//ID3D11Debug* debug;
	//device->QueryInterface(IID_ID3D11Debug, (void**)&debug);
	//
	//viewport.Width = static_cast<float>(appDisplay->screenWidth);
	//viewport.Height = static_cast<float>(appDisplay->screenHeight);
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.MinDepth = 0;
	//viewport.MaxDepth = 1.0f;
	///////////////////////////////
	//viewport2.Width = static_cast<float>(appDisplay2->screenWidth);
	//viewport2.Height = static_cast<float>(appDisplay2->screenHeight);
	//viewport2.TopLeftX = 0;
	//viewport2.TopLeftY = 0;
	//viewport2.MinDepth = 0;
	//viewport2.MaxDepth = 1.0f;
	//
	//viewports[0] = viewport;
	//viewports[1] = viewport2;
	///////////////////////////////
	//context->RSSetViewports(1, &viewport);//2?
	//context->RSSetViewports(1, &viewport2);
	//context->RSSetViewports(2, viewports);
	///////////////////////////////
	//context->OMSetRenderTargets(1, &rtv, nullptr);
	//context->OMSetRenderTargets(1, &rtv2, nullptr);//depthView
	//context->OMSetRenderTargets(2, targets, nullptr);//depthView

	return 0;
}

HRESULT DoubleGame::CreateBackBufers()
{
	//HRESULT res;

	//res = swapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);	//������� �����
	//ZCHECK(res);

	//res = swapChain2->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer2);	//������� �����
	//ZCHECK(res);

	//res = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);			// ������������� ������ ����� ��� ������ ������
	//ZCHECK(res);

	//res = device->CreateRenderTargetView(backBuffer2, nullptr, &rtv2);			// ������������� ������ ����� ��� ������ ������
	//ZCHECK(res);

	//D3D11_TEXTURE2D_DESC depthTexDesc = {};
	//depthTexDesc.ArraySize = 1;//���� �������� � ��� ������
	//depthTexDesc.MipLevels = 1;//������� �������
	//depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;//
	//depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	//depthTexDesc.CPUAccessFlags = 0;
	//depthTexDesc.MiscFlags = 0;
	//depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthTexDesc.Width = appDisplay->screenWidth;
	//depthTexDesc.Height = appDisplay->screenHeight;
	//depthTexDesc.SampleDesc = { 1, 0 };
	//
	//res = device->CreateTexture2D(&depthTexDesc, nullptr, &depthBuffer);
	//ZCHECK(res);
	//
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	//depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthStencilDesc.Flags = 0;
	//
	//res = device->CreateDepthStencilView(depthBuffer, &depthStencilDesc, &depthView);
	//ZCHECK(res);

	return 0;
}

void DoubleGame::Initialize()//�������� ������ � ��������
{
	for (size_t i = 0; i < gameWindows.size(); ++i)
	{
		gameWindows[i]->Initialize();//�������������� ��� ����������
	}

	//������� �����
	//basis = new GridComponent(device, context, {
	//	Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //��� �
	//	Vector4(1.0f,  0.0f,  0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),

	//	Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), //��� �
	//	Vector4(0.0f,  1.0f,  0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),

	//	Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), //��� z
	//	Vector4(0.0f,  0.0f,  1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), },
	//	{ 0, 1, 2, 3, 4, 5 },
	//	gameCamera);

	////�����
	//grid = new GridComponent(device, context, {
	//	Vector4(-4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), //������������ �����
	//	Vector4(-4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-3.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-3.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-2.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-2.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-1.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-1.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(0.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(0.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(1.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(1.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(2.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(2.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(3.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(3.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	//
	//	Vector4(-4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), //�������������� 
	//	Vector4(4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-4.0f, 0.0f, 3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, 3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-4.0f, 0.0f, 2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, 2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-4.0f, 0.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-4.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-4.0f, 0.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-4.0f, 0.0f, -2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, -2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-4.0f, 0.0f, -3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, -3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	},
	//	{ 0,1, 2,3, 4,5, 6,7, 8,9, 10,11, 12,13, 14,15, 16,17, 18,19, 20,21, 22,23, 24,25, 26,27, 28,29, 30,31, 32,33, 34,35 },
	//	gameCamera);

	////�����
	//triangleObject = new TriangleComponent(device, context, {
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
	//	Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	}, { //������ �������� ��� ������
	//	0,1,2, 1,0,3, 4,2,5, 2,4,0, 6,5,7, 5,6,4, 3,7,1, 7,3,6, 4,3,0, 3,4,6, 2,7,5, 7,2,1 }, gameCamera);

	////����� � ���������
	//texObj = new TextureObjComponent(this, device, context, (Vector3::Zero + Vector3(0, -1, 0)), {
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 0.0f),		//0
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f),		//1
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 0.0f, 0.0f),		//2
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 0.0f, 0.0f),		//3
	//	Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 0.0f, 0.0f),		//4
	//	Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f),		//5
	//	Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 0.0f),		//6
	//	Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 0.0f), },		//7
	//	{ 0,1,2, 1,0,3, 4,2,5, 2,4,0, 6,5,7, 5,6,4, 3,7,1, 7,3,6, 4,3,0, 3,4,6, 2,7,5, 7,2,1 },//������ �������� ��� ������
	//	L"Obj.obj", L"WallTex3.png", gameCamera);


	////����� � ��������� � ������
	//texLightObj = new LightObjComponent(this, device, context, (Vector3::Zero + Vector3(0, 1, 0)), {
	//	Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f), },
	//	{ 3,1,0, 2,1,3, 6,4,5, 7,4,6, 11,9,8, 10,9,11, 14,12,13, 15,12,14, 19,17,16, 18,17,19, 22,20,21, 23,20,22 },
	//	L"Obj.obj", L"WallTex.png", gameCamera);

	//components.emplace_back(grid);
	//components.emplace_back(basis);
	//components.emplace_back(triangleObject);
	//components.emplace_back(texObj);
	//components.emplace_back(texLightObj);
}

void DoubleGame::Draw()//���������� ����������
{
	for (size_t i = 0; i < gameWindows.size(); ++i)
	{
		gameWindows[i]->Draw();
	}
}

//void Game::DestroyRecources()
//{
//	//if (backBuffer) backBuffer->Release();
//	//if (context) context->ClearState();
//	//for (auto c : components)
//	//{
//	//	c->DestroyResources();
//	//}
//	//if (rtv) rtv->Release();
//	//if (context) context->Release();
//	//if (device) device->Release();
//}

//void DoubleGame::Update(float deltaTime)//1
//{
//	for (size_t i = 0; i < gameWindows.size(); ++i)
//	{
//		gameWindows[i]->game = Update(deltaTime);
//		
//	}
//	gameCameraCtrl->Update(deltaTime);
//
//	float objVelocity = 3.0f;
//	float lightVelocity = 3.0f;
//
//	//auto mat = Matrix::CreateRotationX(DirectX::XMConvertToRadians(10));
//
//	//����������� �������
//	if (inputDevice->IsKeyDown(Keys::Left))//37
//	{
//		//texLightObj->objectPosition += objVelocity * Vector3::Left * deltaTime;
//		//texLightObj->light_x += lightVelocity * -1 * deltaTime;
//		std::cout << "Move odject left" << std::endl;
//	}
//	if (inputDevice->IsKeyDown(Keys::Right))//39
//	{
//		//texObj->objectPosition += objVelocity * Vector3::Right * deltaTime;
//		//texLightObj->light_x += lightVelocity * 1 * deltaTime;
//		std::cout << "Move odject right" << std::endl;
//	}
//	if (inputDevice->IsKeyDown(Keys::Up))//38
//	{
//		//texObj->objectPosition += objVelocity * Vector3::Up * deltaTime;
//		//texLightObj->light_z += lightVelocity * -1 * deltaTime;
//		std::cout << "Move odject up" << std::endl;
//	}
//	if (inputDevice->IsKeyDown(Keys::Down))//40
//	{
//		//texLightObj->objectPosition += objVelocity * Vector3::Down * deltaTime;
//		//texLightObj->light_z += lightVelocity * 1 * deltaTime;
//		std::cout << "Move odject down" << std::endl;
//	}
//
//	if (inputDevice->IsKeyDown(Keys::Escape))
//	{
//		std::cout << "Exit sas" << std::endl;//
//	}
//
//	Game::Update(deltaTime);//���������� ������ ��� ���� �����������
//}

void DoubleGame::OnMouseMove(InputDevice::MouseMoveEventArgs& args)
{
	//std::cout << args.Position.x << " " << args.Position.y << std::endl; //������������ �������
}