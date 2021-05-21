#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinUser.h>
#include <wrl.h>

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXColors.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#define ZCHECK(exp)                                                        \
    if (FAILED(exp)) {                                                     \
        char buf[1000];                                                     \
        sprintf_s(buf, "Check failed at file: %s at line %i\n", __FILE__, __LINE__); \
        printf(buf);\
        OutputDebugStringA(buf); \
        return 0;                                                          \
    }

//#include "Utils.hpp"
