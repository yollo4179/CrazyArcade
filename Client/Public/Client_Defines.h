#pragma once
#include"process.h"
#include"UIManager.h"

//#include<winsock2.h>
//#include<ws2tcpip.h>
//#pragma comment(lib,"ws2_32")


namespace Client
{
	const static unsigned int g_iWinSizeX = 1920;
	const static unsigned int g_iWinSizeY = 1080;

	enum class LEVEL_ID
	{
		LEVEL_STATIC,
		LEVEL_LOADING,
		LEVEL_MAPTOOL,
		LEVEL_LOGIN,
		LEVEL_LOBY,
		LEVEL_ROOM,
		LEVEL_GAMEPLAY,
		LEVEL_END,


	};

	

}
using namespace Client;
extern HIMC hImc;
extern HINSTANCE g_hInst;
extern HWND g_hWnd;

extern TextSystem::TEXT_INPUT_STATE* g_Text;