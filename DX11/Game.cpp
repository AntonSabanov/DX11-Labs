#include "pch.h"
#include "Game.h"
#include "DisplayWindow.h"
#include "TriangleComponent.h"

using namespace DirectX;

//Game::GetInstance()
//{
//	return Instance;
//}

Game::Game(DisplayWindow* display, std::string name)
{
	Game::appDisplay = display;
	Game::name = name;
	//Instance = this;
}

//void Game::SetInstance(Game* gameObj)
//{
//	gameObj->Instance = gameObj;
//}
//
//Game* Game::GetInstance()
//{
//	return Game::this->Instance;
//}

HRESULT Game::Run() //����������� �������� � ������ �����
{
	PrepareRecources();//�������������� ���������

	std::vector<TriangleComponent*> triangles;
	triangles.emplace_back(new TriangleComponent(device, context, { XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
																	XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
																	XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		}));
	triangles.emplace_back(new TriangleComponent(device, context, { XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
																	XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����		
																	XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		}));
	

	
	//Initialize();//����� ������������� � ������ ����������, ��� ����� ������� ����������, ���������� ����� ���������
	//for (auto comp : components) comp->Initialize();//����� ����������� �� ���� ����������� � �������� � ��� ���������� � ������� ����� ��������� ������� � �.�

	
	//-----------------------------------------------------------------------------
	//CREATE WINDOW MESSAGE LOOP
	//-----------------------------------------------------------------------------
	MSG msg = {};

	// Loop until there is a quit message from the window or the user.
	bool isExitRequested = false;
	while (!isExitRequested) {
		// Handle the windows messages.
		/*while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);*/
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}


#pragma region DrawSomeStaff
		//Draw();//update internal

#pragma region Preparef
		auto	curTime = std::chrono::steady_clock::now();//������� �����
		float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;//������� ������� ������ � ����������� �����
		PrevTime = curTime;

		totalTime += deltaTime;
		frameCount++;//�������, ������� ������� ������

		if (totalTime > 1.0f) {
			float fps = frameCount / totalTime;//������� ���

			totalTime = 0.0f;

			WCHAR text[256];
			swprintf_s(text, TEXT("FPS: %f"), fps);
			SetWindowText(appDisplay->hWnd, text); //������� ��� ������ �������� ������

			frameCount = 0;
		}

		//-----------------------------------------------------------------------------
		//CLEAR BACKBUFER
		//-----------------------------------------------------------------------------
		float color[] = { totalTime, 0.1f, 0.1f, 1.0f };
		//float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };//����, ������� �� ������� ������ ������ ���

		context->OMSetRenderTargets(1, &rtv, nullptr);//��� ��� � ��� ���� ������, ���� ��� ������ �� ������ �����//���������� ������ ������� ������ � ��������� ���������� (������� ������� ������)
		context->ClearRenderTargetView(rtv, color);//�������� ������
#pragma endregion PrepareFrame

#pragma region Draw
		D3D11_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(appDisplay->screenWidth);
		viewport.Height = static_cast<float>(appDisplay->screenHeight);
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		context->RSSetViewports(1, &viewport);
		context->OMSetRenderTargets(1, &rtv, nullptr);

		annotation->BeginEvent(L"BeginDraw");
		//context_->DrawIndexed(6, 0, 0);

		for (auto&& i : triangles) 
		{
			i->Draw(context);
		}
		annotation->EndEvent();

#pragma endregion Draw
		//comp->Draw(context);

		//-----------------------------------------------------------------------------
		//PRESENT RESULT
		//-----------------------------------------------------------------------------
	//EndFrame
		swapChain1->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0); //������� � �������� ����� (�� �����) ���������� � ������ ������ //EndFrame
		
#pragma endregion DrawSomeStaff
	}

	DestroyRecources();
	return 0;
}

//void Initialize()
//{
//	//������� ����� ���������� ������� ����������� � ��������� �� � components 
//	//����� ������������� � ������ ����������, ��� ����� ������� ����������, ���������� ����� ���������
//	//����� ��������� �� ���� ����������� � �������� � ��� ���������
//}

HRESULT Game::PrepareRecources()
{
	HRESULT res; //���� ������������ ��������� ��������

	//-----------------------------------------------------------------------------
	//��������� ���������� ����� ��������� ����������
	//-----------------------------------------------------------------------------
	//���������, ����������� ���� ����� (�������� ��������� ������) ���������, �� ������� ����� ����������� ����������
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;//� ��� 2 ������ ������(������ ��� � FLIP_DISCARD ������ ���� ��� ������� 2 ������)
	swapDesc.BufferDesc.Width = appDisplay->screenWidth;//������ ��������� ������	
	swapDesc.BufferDesc.Height = appDisplay->screenHeight;//������ ��������� ������
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//������ ������� � ������
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;//������� ���������� ������
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;//� �������
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//���������� ������ ��� ����
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//���������� ������ - ���������� � ���� ��� ��� �� ���������
	swapDesc.OutputWindow = appDisplay->hWnd;//�������� � ������ ���� WinApi
	swapDesc.Windowed = true;//�� ������������� �����
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//����������, ��� ���������� � ���������� ��� ������ ����� ������������
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//�������� �� ����������� ������� ��� �������
	swapDesc.SampleDesc.Count = 1; //�������� �� �������������� � ������� �� ��������� SwapEffect
	swapDesc.SampleDesc.Quality = 0; //��. ����

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 }; //�������������� ����
	//�������� �������� �� �������� ��� �������, ������� ������ ��� ��, ��� �� ����������� � ����� present � ��������� ������� �� �����
	res = D3D11CreateDeviceAndSwapChain(
		nullptr,//default adapter
		D3D_DRIVER_TYPE_HARDWARE,//videocard driver
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,//11_0
		1,
		D3D11_SDK_VERSION,
		&swapDesc,//��������� ��� ���������
		&swapChain,//��� ��������
		&device,//������
		nullptr,//��������� �� ������ �������������� ������� � ������� featureLefel
		&context);//�������� ����������
	ZCHECK(res);//��������

	CreateBackBufer();//������� ������ �����

	swapChain->QueryInterface<IDXGISwapChain1>(&swapChain1);

	context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation);
	
	ID3D11Debug* debug;
	device->QueryInterface(IID_ID3D11Debug, (void**)&debug);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(appDisplay->screenWidth);
	viewport.Height = static_cast<float>(appDisplay->screenHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &rtv, nullptr);

	/*ID3D11Debug* debug;
	device->QueryInterface(IID_ID3D11Debug, (void**)&debug);*/

	return 0;
}

HRESULT Game::CreateBackBufer()
{
	HRESULT res;

	res = swapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);	//������� �����
	ZCHECK(res);
	res = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);			// ������������� ������ ����� ��� ������ ������
	ZCHECK(res);
	return 0;
}

void Game::Draw()
{
	//TriangleComponent* comp = new TriangleComponent();
	//comp->Initialize(device, context);
	//components = { comp };
#pragma region PrepareFrame
	auto	curTime = std::chrono::steady_clock::now();//������� �����
	float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;//������� ������� ������ � ����������� �����
	PrevTime = curTime;

	totalTime += deltaTime;
	frameCount++;//�������, ������� ������� ������


	if (totalTime > 1.0f) {
		float fps = frameCount / totalTime;//������� ���

		totalTime = 0.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(appDisplay->hWnd, text); //������� ��� ������ �������� ������

		frameCount = 0;
	}

	//-----------------------------------------------------------------------------
	//CLEAR BACKBUFER
	//-----------------------------------------------------------------------------
	float color[] = { totalTime, 0.1f, 0.1f, 1.0f };
	//float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };//����, ������� �� ������� ������ ������ ���
	//D3D11_VIEWPORT viewport = {};//��������� ��������� ��� rastState
	//viewport.Width = static_cast<float>(display->screenWidth);
	//viewport.Height = static_cast<float>(game->appDisplay->screenHeight);
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.MinDepth = 0;
	//viewport.MaxDepth = 1.0f;
	//context->ClearState();//
	context->OMSetRenderTargets(1, &rtv, nullptr);//��� ��� � ��� ���� ������, ���� ��� ������ �� ������ �����//���������� ������ ������� ������ � ��������� ���������� (������� ������� ������)
	context->ClearRenderTargetView(rtv, color);//�������� ������
#pragma endregion PrepareFrame

#pragma region Draw
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(appDisplay->screenWidth);
	viewport.Height = static_cast<float>(appDisplay->screenHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &rtv, nullptr);

	annotation->BeginEvent(L"BeginDraw");
	//context_->DrawIndexed(6, 0, 0);

	//for (auto&& i : components) 
	//{
	//	i->Draw(context);
	//}
	annotation->EndEvent();
	
#pragma endregion Draw
	//comp->Draw(context);

	//-----------------------------------------------------------------------------
	//PRESENT RESULT
	//-----------------------------------------------------------------------------
//EndFrame
	swapChain1->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0); //������� � �������� ����� (�� �����) ���������� � ������ ������ //EndFrame
}

//void Game::Update()
//{
//	//for (auto comp : components) comp->Update();
//}

void Game::DestroyRecources()
{
	backBuffer->Release();
	if (context) context->ClearState();
	//if (_constantBuffer) _constantBuffer->Release();
	//if (vertexBuffer) vertexBuffer->Release();
	//if (indexBuffer) indexBuffer->Release();
	/*for (auto c : Components)
	{
		c->DestroyResources();
	}*/
	if (rtv) rtv->Release();
	if (context) context->Release();
	if (device) device->Release();
	//-----------------------------------------------------------------------------
	//CLEAN THE MESS
	//-----------------------------------------------------------------------------
	/*vertexShader->Release();
	pixelShader->Release();

	device->Release();

	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	return 0;*/
}