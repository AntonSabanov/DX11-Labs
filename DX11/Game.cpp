#include "pch.h"
#include "Game.h"
#include "DisplayWindow.h"
#include "TriangleComponent.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

Game* gameInstance;

Game::Game(DisplayWindow* display, std::string name)
{
	gameInstance = this;
	Game::appDisplay = display;
	Game::name = name;
}

HRESULT Game::Run() //определение ресурсов и запуск цикла
{
	PrepareRecources();//инициализируем параметры
	Initialize();//инициалзируем точки
	
	//Initialize();//будет переопределен в классе наследнике, там будут созданы компоненты, компоненты будут добавлены
	//for (auto comp : components) comp->Initialize();//далее пробегаемся по всем компонентам и вызываем у них инишеолайз в которых будут прочитаны шейдера и т.д
	
	//-----------------------------------------------------------------------------
	//CREATE WINDOW MESSAGE LOOP
	//-----------------------------------------------------------------------------
	MSG msg = {};

	// Loop until there is a quit message from the window or the user.
	bool isExitRequested = false;
	while (!isExitRequested) 
	{
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		//	TranslateMessage(&msg);
		//	DispatchMessage(&msg);
		//}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) 
		{
			isExitRequested = true;
		}


#pragma region DrawSomeStaff
		Draw();//update internal

	}

	DestroyRecources();
	return 0;
}

void Game::Initialize()
{
	//Camera* gameCamera = new Camera(this);//
	components.emplace_back(new TriangleComponent(device, context, 
		{ Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		}, { //массив индексов для кубика
		0,1,2,
		1,0,3,
		4,2,5,
		2,4,0,
		6,5,7,
		5,6,4,
		3,7,1,
		7,3,6,
		4,3,0,
		3,4,6,
		2,7,5,
		7,2,1 },
		nullptr));//камера

	//components.emplace_back(new TriangleComponent(device, context, { Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет
	//															Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//															Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	}));
	//components.emplace_back(new TriangleComponent(device, context, { Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//																Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет		
	//																Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	}));
	
	//создаем энное количество триангл компонентов и добовляем их в components 
	//будет переопределен в классе наследнике, там будут созданы компоненты, компоненты будут добавлены
	//затем пробигаем по всем компонентам и вызываем у них инишалайз
}

HRESULT Game::PrepareRecources()
{
	HRESULT res; //сюда записывается результат проверок

	//-----------------------------------------------------------------------------
	//Установка параметров перед созданием устройства
	//-----------------------------------------------------------------------------
	//структура, описывающая цепь связи (свойства переднего буфера) параметры, по которым будет создаваться устройство
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;//у нас 2 задних буфера(потому что у FLIP_DISCARD должно быть как минимум 2 буфера)
	swapDesc.BufferDesc.Width = appDisplay->screenWidth;//ширина переднего буфера	
	swapDesc.BufferDesc.Height = appDisplay->screenHeight;//высота переднего буфера
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//формат пикселя в буфере
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;//частота обновления экрана
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;//в секунду
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//подстройка буфера под окно
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//назначение буфера - записывать в него все что мы посчитали
	swapDesc.OutputWindow = appDisplay->hWnd;//привязка к нашему окну WinApi
	swapDesc.Windowed = true;//не полноэкранный режим
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//определяет, что происходит с содержимым бэк буфера после переключения
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//отвечает за возможность ресайза бэк буферов
	swapDesc.SampleDesc.Count = 1; //отвечает за мультисэмплинг и зависит от параметра SwapEffect
	swapDesc.SampleDesc.Quality = 0; //см. выше

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 }; //поддерживаемые фичи
	//свапчейн отвечает за создание бэк буферов, которые хранят все то, что мы отрендерили и после present с бэкбуфера попадет на экран
	res = D3D11CreateDeviceAndSwapChain(
		nullptr,//default adapter
		D3D_DRIVER_TYPE_HARDWARE,//videocard driver
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,//11_0
		1,
		D3D11_SDK_VERSION,
		&swapDesc,//параметры для свапчейна
		&swapChain,//сам свапчейн
		&device,//девайс
		nullptr,//указатель на первый поддерживаемый элемент в массиве featureLefel
		&context);//контекст устройства
	ZCHECK(res);//проверка

	CreateBackBufer();//создаем задний буфер

	swapChain->QueryInterface<IDXGISwapChain1>(&swapChain1);

	context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation);
	
	ID3D11Debug* debug;
	device->QueryInterface(IID_ID3D11Debug, (void**)&debug);

	//D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(appDisplay->screenWidth);
	viewport.Height = static_cast<float>(appDisplay->screenHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
	//context->OMSetRenderTargets(1, &rtv, nullptr);
	context->OMSetRenderTargets(1, &rtv, depthView);

	/*ID3D11Debug* debug;
	device->QueryInterface(IID_ID3D11Debug, (void**)&debug);*/

	return 0;
}

HRESULT Game::CreateBackBufer() //создание заднего буфера и буфера глубины
{
	HRESULT res;

	res = swapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);	//создаем буфер
	ZCHECK(res);
	res = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);			// устанавливаем задний буфер как рендер таргет
	ZCHECK(res);

	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;//одна текстура в бэк буфере
	depthTexDesc.MipLevels = 1;//нулевой уровень
	depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;//
	depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = appDisplay->screenWidth;
	depthTexDesc.Height = appDisplay->screenHeight;
	depthTexDesc.SampleDesc = { 1, 0 };

	res = device->CreateTexture2D(&depthTexDesc, nullptr, &depthBuffer);
	ZCHECK(res);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = 0;

	res = device->CreateDepthStencilView(depthBuffer, &depthStencilDesc, &depthView);
	ZCHECK(res);

	return 0;
}

void Game::Draw()
{
#pragma region PrepareFrame
	auto	curTime = std::chrono::steady_clock::now();//текущее время
	float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;//сколько времени прошло с предыдущего кадра
	PrevTime = curTime;

	totalTime += deltaTime;
	frameCount++;//считаем, сколько времени прошло

	if (totalTime > 1.0f) {
		float fps = frameCount / totalTime;//считаем фпс

		totalTime = 0.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(appDisplay->hWnd, text); //выводим фпс вместо названия окошка

		frameCount = 0;
	}

	//-----------------------------------------------------------------------------
	//CLEAR BACKBUFER
	//-----------------------------------------------------------------------------
	//float color[] = { totalTime, 0.1f, 0.1f, 1.0f };
	float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };//цвет, которым мы очищаем рендер таргет вью
	
	context->OMSetRenderTargets(1, &rtv, depthView);
	//context->OMSetRenderTargets(1, &rtv, nullptr);//так как у нас флип модель, надо это делать на каждом кадре//Подключаем объект заднего буфера к контексту устройства (очистка заднего буфера)
	context->ClearRenderTargetView(rtv, color);//очистили цветом
	context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
#pragma endregion PrepareFrame

#pragma region Draw

	context->RSSetViewports(1, &viewport);
	//context->OMSetRenderTargets(1, &rtv, depthView);

	annotation->BeginEvent(L"BeginDraw");
	//context_->DrawIndexed(6, 0, 0);

	Update(deltaTime);

	//for (auto&& i : components)
	//{
	//	i->Draw(context);
	//}
	annotation->EndEvent();
	
#pragma endregion Draw
	//-----------------------------------------------------------------------------
	//PRESENT RESULT
	//-----------------------------------------------------------------------------
//EndFrame
	//swapChain1->Present(1, DXGI_PRESENT_DO_NOT_WAIT); //вывести в передний буфер (на экран) информацию в заднем буфере //EndFrame
	swapChain1->Present(1, 0); //вывести в передний буфер (на экран) информацию в заднем буфере //EndFrame
}

void Game::Update(float deltaTime)//3
{
	for (auto comp : components)
	{
		comp->Update(deltaTime);
		comp->Draw(context);
		//swapChain1->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0); //вывести в передний буфер (на экран) информацию в заднем буфере //EndFrame
	}
}

void Game::DestroyRecources()
{
	backBuffer->Release();
	if (context) context->ClearState();
	//if (constantBuffer) constantBuffer->Release();
	//if (vertexBuffer) vertexBuffer->Release();
	//if (indexBuffer) indexBuffer->Release();
	for (auto c : components)
	{
		c->DestroyResources();
	}
	if (rtv) rtv->Release();
	if (context) context->Release();
	if (device) device->Release();
	//-----------------------------------------------------------------------------
	//CLEAN THE MESS
	//-----------------------------------------------------------------------------


	//debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}

//обработка ввода
LRESULT CALLBACK Game::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)//вызывается каждый раз, когда система получает сообщение (нужно для окошка)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		//isExitRequested = true;
		return 0;
	}

	case WM_SIZE:
	{
		std::cout << "Width " << LOWORD(lparam) << " Height " << HIWORD(lparam) << std::endl;

		return 0;
	}

	// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		return 0;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == nullptr) {
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
			//	raw->data.keyboard.MakeCode,
			//	raw->data.keyboard.Flags,
			//	raw->data.keyboard.Reserved,
			//	raw->data.keyboard.ExtraInformation,
			//	raw->data.keyboard.Message,
			//	raw->data.keyboard.VKey);

			gameInstance->inputDevice->OnKeyDown({
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.VKey,
				raw->data.keyboard.Message
				});
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			gameInstance->inputDevice->OnMouseMove({
				raw->data.mouse.usFlags,
				raw->data.mouse.usButtonFlags,
				static_cast<int>(raw->data.mouse.ulExtraInformation),
				static_cast<int>(raw->data.mouse.ulRawButtons),
				static_cast<short>(raw->data.mouse.usButtonData),
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY
				});
		}

		delete[] lpb;
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// All other messages pass to the message handler in the system class.
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}