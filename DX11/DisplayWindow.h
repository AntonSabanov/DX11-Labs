#pragma once

#include "pch.h"
//#include "InputDevice.h"//

class DisplayWindow
{
public:
	int screenWidth = 0;
	int screenHeight = 0;
	HINSTANCE hInst = nullptr;
	HWND hWnd = nullptr;
	WNDCLASSEX wc = {};

	//static InputDevice* inDevice;//
	//static HMODULE module;

public:
	//static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	DisplayWindow(int width, int height);

};

