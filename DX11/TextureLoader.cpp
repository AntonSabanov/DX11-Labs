#include "pch.h"
#include "Game.h"
#include "TextureLoader.h"


TextureLoader::TextureLoader(Game* inGame)
{
	game = inGame;

	CoInitialize(NULL); //инициализация работы с ком объектами

	// Create the COM imaging factory
	//создаем ком объект для фабрики
	HRESULT res = CoCreateInstance(CLSID_WICImagingFactory2, 
									NULL, 
									CLSCTX_INPROC_SERVER, 
									IID_PPV_ARGS(&factory)); 
	//ZCHECK(res);
}

TextureLoader::~TextureLoader()
{
	factory->Release();
}

//на выходе получим текстуру и шейдер ресурс вью
HRESULT TextureLoader::LoadTextureFromFile(LPCWSTR fileName, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureView, bool generateMips, bool useSrgb, UINT frameIndex)
{
	//factory->CreateDecoder(IID_IWICBitmapDecoder, nullptr, &decoder);
	//IWICStream* pIWICStream = nullptr;
	//factory->CreateStream(&pIWICStream);
	//
	//pIWICStream->InitializeFromFilename(fileName, GENERIC_READ);

	IWICBitmapDecoder* decoder; //декодирование файла

	factory->CreateDecoderFromFilename(fileName, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

	IWICBitmapFrameDecode* frame;// кадр из файла (если гифка)
	auto hr = decoder->GetFrame(frameIndex, &frame);

	IWICFormatConverter* converter;//конвертор для формата
	factory->CreateFormatConverter(&converter);

	converter->Initialize(frame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);//инициализируем конвертер

	UINT width, height;
	converter->GetSize(&width, &height); //ширина и высота кадра

	UINT stride = width * 4;//ширина 4 канала (количество элементов в строке)
	UINT bufSize = stride * height;//размер буфера (количество элементов в текстуре)

	BYTE* buf = new BYTE[bufSize]; //создаем буфер

	converter->CopyPixels(nullptr, stride, bufSize, buf); //копируем в буфер пиксель


	D3D11_TEXTURE2D_DESC desc = {};//дескриптор для текстуры
	desc.Width = width;//ширина
	desc.Height = height;//высота
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.Format = useSrgb ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.MipLevels = generateMips ? 0 : 1;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buf;
	data.SysMemPitch = stride;//количество элементов в строке
	data.SysMemSlicePitch = bufSize;//всего элементов

	game->device->CreateTexture2D(&desc, generateMips ? nullptr : &data, &texture);//создаем текстуру
	game->device->CreateShaderResourceView(texture, nullptr, &textureView);

	if (generateMips) //если генерируем мипы
	{
		game->context->UpdateSubresource(texture, 0, nullptr, buf, stride, bufSize);
		game->context->GenerateMips(textureView);
	}

	decoder->Release();
	frame->Release();
	converter->Release();
	delete[] buf;

	return S_OK;
}