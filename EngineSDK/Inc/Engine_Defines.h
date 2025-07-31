#pragma once
#pragma warning (disable : 4251)
#pragma warning(disable : 5208)
#pragma warning(disable : 4267)
#pragma warning(disable : 4091)
#pragma warning(disable : 4244)
#pragma warning(disable : 4819)

#include <d3d11.h>
#include <wrl/client.h> // ComPtr
using  Microsoft::WRL::ComPtr;

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "Effects11\d3dx11effect.h"//셰이더
#include "DirectXTK\DDSTextureLoader.h"//텍스초
#include "DirectXTK\WICTextureLoader.h"//텍스처
#include "DirectXTK\ScreenGrab.h"
#include "DirectXTK\PrimitiveBatch.h"// 디버그 
#include "DirectXTK\VertexTypes.h"
#include "DirectXTK\Effects.h" //셰이더

#include "DirectXTK\SpriteBatch.h"
#include "DirectXTK\SpriteFont.h"
#include <d3dcompiler.h>//셰이더 컴파일러
#ifdef _DEBUG
#pragma comment(lib, "DirectXTKd.lib")
#pragma comment(lib, "Effects11d.lib")
#else 
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "Effects11.lib")
#endif

#ifdef _DEBUG
#pragma region ImGui
#define IMGUI_DEFINE_MATH_OPERATORS_IMPLEMENTED
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
#define IMAPP_IMPL
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#pragma endregion ImGui

#pragma comment(lib, "imgui.lib")
#pragma comment(lib, "imgui_dll.lib")
#pragma comment(lib, "imm32.lib")
#endif

#include <../../EngineSDK/Json/json.hpp>
using namespace nlohmann;


#include <windows.h>  // 먼저 포함
#include <mmsystem.h> // winmm.lib 필요
#include"winerror.h"
#include"minwindef.h"

#include <dsound.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")



#pragma	comment(lib, "dinput8.lib")
#pragma	comment(lib, "dxguid.lib")


#include <iostream>
#include <fstream>
#include <filesystem>

#include <deque>
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <cmath>
#include <bitset>
#include<queue>
#include<memory>
#include<functional>
#pragma execution_character_set("utf-8")

using namespace std;


#include"Engine_Typedef.h"
#include"Engine_Macros.h"
#include"Engine_FDs.h"
#include"Engine_Structs.h"
#include"Engine_Functions.h"


using namespace Engine;




#pragma region  디버그 창 
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif
#pragma endregion 디버그 창 