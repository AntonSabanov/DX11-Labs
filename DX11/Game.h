#pragma once
#include "pch.h"
#include "DisplayWindow.h"
#include "GameComponent.h"

class Game
{
public:
	DisplayWindow* appDisplay = nullptr;//���� ����������
	//GameComponent* components;
	ID3D11Texture2D* backBuffer = nullptr; //������ ����� � ���� ��������										//?
	ID3D11DeviceContext* context = nullptr;//���������
	ID3DUserDefinedAnnotation* annotation = nullptr; // debug annotation
	Microsoft::WRL::ComPtr<ID3D11Device> device;//������
	////Instance
	////Name
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
	Game(DisplayWindow* display);

	HRESULT Run();
	//virtual void Initialize(); //������� ����� ���������� ������� ����������� � ��������� �� � components //����� ������������� � ������ ����������, ��� ����� ������� ����������, ���������� ����� ���������//����� ��������� �� ���� ����������� � �������� � ��� ���������
	virtual HRESULT PrepareRecources();
	virtual void DestroyRecources();
	HRESULT CreateBackBufer();
	virtual void Update();// ��������� �� ���� ����������� � �������� � ��� ����� ������
	virtual void Draw(); // ��������� �� ���� ����������� � �������� � ��� ����� draw

	//static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


};

