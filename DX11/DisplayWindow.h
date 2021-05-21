#pragma once

#include "pch.h"

class DisplayWindow
{
public: 
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	DisplayWindow(int width, int height);

	//DisplayWindow(int width, int height, WNDPROC wndProc);
	//~DisplayWindow();
	
//
public:
	int screenWidth = 0;
	int screenHeight = 0;

public:
	HINSTANCE hInst = nullptr;
	HWND hWnd = nullptr;
	WNDCLASSEX wc = {};

	//static HMODULE module;
};

