// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#pragma once

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#define WIN32_LEAN_AND_MEAN   

#include<stdlib.h>
#include<malloc.h>
#include<memory.h>
#include<windows.h>//TCHAR, 
#include<tchar.h>//For Version
#include<process.h>
#include<memory>
static int a = 3;

#include <imm.h>
#pragma comment(lib, "imm32.lib")

#include"Libs.h"
#include"AbstractFactorySystem.h"

/*서버 라이브러리 */

#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32")
//#pragma comment(lib, "DirectXTK.lib")
//#pragma comment(lib, "DirectXTKd.lib")
//#pragma comment(lib, "Effects11.lib")
//#pragma comment(lib, "Effects11d.lib")
//#pragma comment(lib, "imgui.lib")
//#pragma comment(lib, "imgui_dll.lib")
//#pragma comment(lib, "imm32.lib")
#endif //PCH_H
