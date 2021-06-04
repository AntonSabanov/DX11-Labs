#pragma once
#include "pch.h"
#include "wincodec.h"

//#include "Exports.h"

class Game;

class TextureLoader
{
	IWICImagingFactory2* factory;
	Game* game;

public:
	TextureLoader(Game* inGame);
	~TextureLoader();

	HRESULT LoadTextureFromFile(LPCWSTR fileName,						//имя файла
								ID3D11Texture2D*& texture,				//указатель на текутуру
								ID3D11ShaderResourceView*& textureView, 
								bool generateMips = false, 
								bool useSrgb = false, 
								UINT frameIndex = 0);
};



