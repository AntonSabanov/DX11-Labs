#pragma once
#include "pch.h"
#include "DisplayWindow.h"
#include "GameComponent.h"
#include "TriangleComponent.h"
#include <vector>

class Game
{
//private:
//	Game* Instance;
public:
	std::vector<GameComponent*> trianglObjects;
	//TriangleComponent sas[8];
	//Game* Instance;
	std::string name;
	DisplayWindow* appDisplay = nullptr;//���� ����������
	D3D11_VIEWPORT viewport = {};
	ID3D11Texture2D* backBuffer = nullptr; //������ ����� � ���� ��������										//?
	ID3D11DeviceContext* context = nullptr;//���������
	ID3DUserDefinedAnnotation* annotation = nullptr; // debug annotation
	ID3D11Device* device;//������ Microsoft::WRL::ComPtr<ID3D11Device>
	////Instance
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	////RenderSRV
	ID3D11RenderTargetView* rtv = nullptr;//������ ������� ������, � ������� ����� �������� //RenderView		//?
	////ScreenResized
	////StartTime
	IDXGISwapChain* swapChain = nullptr;//������ � ��������, ����� �� �����
	IDXGISwapChain1* swapChain1 = nullptr;
	float totalTime = 0;
	unsigned int frameCount = 0;


public:
	Game(DisplayWindow* display, std::string name);
	
	//static void SetInstance(Game* gameObj);
	//static Game* GetInstance();
	HRESULT Run();
	virtual void Initialize(); //������� ����� ���������� ������� ����������� � ��������� �� � components //����� ������������� � ������ ����������, ��� ����� ������� ����������, ���������� ����� ���������//����� ��������� �� ���� ����������� � �������� � ��� ���������
	virtual HRESULT PrepareRecources();
	HRESULT CreateBackBufer();
	virtual void Draw(); // ��������� �� ���� ����������� � �������� � ��� ����� draw
	virtual void DestroyRecources();
	
	virtual void Update(float deltaTime);// ��������� �� ���� ����������� � �������� � ��� ����� ������


};

