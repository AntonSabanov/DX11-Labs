#include "DoubleGame.h"
#include "SimpleMath.h"
#include "GridComponent.h"
#include "TextureObjComponent.h"
#include "LightObjComponent.h"
#include "Keys.h"
#include "Game.h"

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
		gameWindows[i]->CreateLocalRecources();//создаем свапчейны, вьюпорты ртв и тд.
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

	DestroyRecources();
	return 0;
}

HRESULT DoubleGame::PrepareRecources()//создание девайса и контекста
{
	HRESULT res; 

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 }; //поддерживаемые фичи
	
	res = D3D11CreateDevice(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE, 
		nullptr, 
		D3D11_CREATE_DEVICE_DEBUG, 
		featureLevel,													//11_0
		1,
		D3D11_SDK_VERSION, 
		&device, 
		nullptr,														//указатель на первый поддерживаемый элемент в массиве featureLefel
		&context);

	for (size_t i = 0; i < gameWindows.size(); ++i)
	{
		gameWindows[i]->device = device;
		gameWindows[i]->context = context;
	}
	
	return 0;
}

HRESULT DoubleGame::CreateBackBufers()
{
	//HRESULT res;

	//res = swapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);	//создаем буфер
	//ZCHECK(res);

	//res = swapChain2->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer2);	//создаем буфер
	//ZCHECK(res);

	//res = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);			// устанавливаем задний буфер как рендер таргет
	//ZCHECK(res);

	//res = device->CreateRenderTargetView(backBuffer2, nullptr, &rtv2);			// устанавливаем задний буфер как рендер таргет
	//ZCHECK(res);

	//D3D11_TEXTURE2D_DESC depthTexDesc = {};
	//depthTexDesc.ArraySize = 1;//одна текстура в бэк буфере
	//depthTexDesc.MipLevels = 1;//нулевой уровень
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

void DoubleGame::Initialize()//создание камеры и объектов
{
	for (size_t i = 0; i < gameWindows.size(); ++i)
	{
		gameWindows[i]->Initialize();//инициализируем все компоненты
	}
}

void DoubleGame::Draw()//глобальный отрисовщик
{
	for (size_t i = 0; i < gameWindows.size(); ++i)
	{
		gameWindows[i]->Draw();
	}
}

void DoubleGame::DestroyRecources()
{
	if (device) device->Release();
	if (context) context->ClearState();
	if (context) context->Release();
	if (factory) factory->Release();
	if (adapter) adapter->Release();
	if (superDevice) superDevice->Release();
	gameWindows[0]->Game::DestroyRecources();

	/*for (auto c : gameWindows)
	{
		c->Game::DestroyRecources();
	}*/
}

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
//	//перемещение объекта
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
//	Game::Update(deltaTime);//вызывкется апдейт для всех компонентов
//}

void DoubleGame::OnMouseMove(InputDevice::MouseMoveEventArgs& args)
{
	//std::cout << args.Position.x << " " << args.Position.y << std::endl; //отслеживание курсора
}