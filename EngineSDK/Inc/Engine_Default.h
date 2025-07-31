#pragma once
#pragma warning (disable : 4251)
#pragma warning(disable : 5208)
#pragma warning(disable : 4267)
#pragma warning(disable : 4091)
#pragma warning(disable : 4244)
#pragma warning(disable : 4819)





#include <windows.h>  // 먼저 포함
#include <mmsystem.h> // winmm.lib 필요

#include <dsound.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

#define DIRECTINPUT_VERSION	0x0800

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

using namespace std;

#include"Engine_Typedef.h"
#include"Engine_Defines.h"
#include"Engine_Functions.h"
#include"Engine_Macros.h"
#include"Engine_Structs.h"

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