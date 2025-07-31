#include"pch.h"
#include "Loader.h"
#include"GameInstance.h"
#include"LevelLoginHeaders.h"
#include"LevelRoomHeaders.h"
#include"Prototype_Static.h"
#include"LevelMaptoolHeaders.h"
#include"TestGameObject.h"
CLoader::CLoader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:m_pDevice{ pDevice }, m_pContext{ pContext }, m_IsFinished{false}
	, m_pGameInstance{CGameInstance::GetInstance()}
{
}

CLoader::~CLoader()
{
	DeleteCriticalSection(&m_CriticalSection);
}
_uint WINAPI EntryPoint(void* pArg)
{
	CLoader* m_pLoader = reinterpret_cast<CLoader*>(pArg);
	if (FAILED(m_pLoader->Loading()))
		return 0;
	return true;
}


HRESULT CLoader::Initialize(LEVEL_ID NextLevel)
{
	m_iLevelId = NextLevel;
	InitializeCriticalSection(&m_CriticalSection);
	DWORD dwThreadHandle = {};
	m_hThread = (HANDLE)
		_beginthreadex(
			nullptr,
			0,//default  stk size
			EntryPoint,
			this,
			0,
			nullptr
		);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);
	CoInitializeEx(nullptr, 0);
	switch (m_iLevelId)
	{
		/*For Proto*/
	case LEVEL_ID::LEVEL_LOGIN:
		Loading_LevelLogin();
		break;
	case LEVEL_ID::LEVEL_LOBY:
		Loading_LevelLoby();
		break;
#ifdef _DEBUG
	case LEVEL_ID::LEVEL_MAPTOOL:
		Loading_LevelMapTool();
		break;
#endif
	case LEVEL_ID::LEVEL_ROOM:
		Loading_LevelRoom();
		break;
	case LEVEL_ID::LEVEL_GAMEPLAY:
		Loading_LevelGamePlay();
		break;
	case LEVEL_ID::LEVEL_STATIC:
		Loading_LevelStatic();
		break;
	}
	m_IsFinished = true;
	
	LeaveCriticalSection(&m_CriticalSection);
	return S_OK;
}

HRESULT CLoader::Loading_LevelLogin()
{

	/*Add Texture*/
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_LOGIN,
		L"PROTO.TEXTURE.UI_LOGIN",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Login/Login.png"),1)
	);


	/*원본 등록*/
#pragma region  로그인 백그라운드 
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_LOGIN, L"PROTO.OBJECT.LOGIN.BG", CLoginBG::Create(m_pDevice, m_pContext));
	CLoginBG::LOGINBG_DESC  tDesc1 = {};
	tDesc1.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc1.fSpeedPerSec = 10.f;
	tDesc1.ObjectPosition = { 0.f,0.f,1.f };
	tDesc1.ObjectSize = { g_iWinSizeX,g_iWinSizeY,0.f};
	tDesc1._ZOrder = 0;
	tDesc1._bSelected = false;
	tDesc1._UIMode = UIMODE::UI_FIXED;
	tDesc1._UIState = UISTATE::UI_RELEASED;
	/*Add GameObject To Layer */
	m_pGameInstance->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_LOGIN, L"PROTO.OBJECT.LOGIN.BG", (_uint)LEVEL_ID::LEVEL_LOGIN, L"LOGIIN_BG", &tDesc1);
#pragma endregion  로그인 백그라운드 


#pragma region 로그인 패널 
	
	/*프로토 등록*/
	/*아이디 박스 */
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_LOGIN, L"PROTO.OBJECT.LOGIN.TEXT_ID", CLoginIDBox::Create(m_pDevice, m_pContext));
	/* 비밀번호 박스*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_LOGIN, L"PROTO.OBJECT.LOGIN.TEXT_PASSWORD", CLoginPasswordBox::Create(m_pDevice, m_pContext));
	/* 로그인 버튼*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_LOGIN, L"PROTO.OBJECT.LOGIN.BUTTON", CLoginButton::Create(m_pDevice, m_pContext));
	/* 패널 */
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_LOGIN, L"PROTO.OBJECT.LOGIN.PANNEL", CLoginPannel::Create(m_pDevice, m_pContext));
	
	
	CLoginPannel::LOGINPANNEL_DESC  tDesc = {};
	tDesc.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc.fSpeedPerSec = 10.f;
	tDesc.ObjectPosition = { 0.f,0.f,1.f };
	tDesc.ObjectSize = { g_iWinSizeX,g_iWinSizeY,0.f };
	tDesc._ZOrder = 0;
	tDesc._bSelected = false;
	tDesc._UIMode = UIMODE::UI_FIXED;
	tDesc._UIState = UISTATE::UI_RELEASED;
	/*Add GameObject To Layer */
	m_pGameInstance->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_LOGIN, L"PROTO.OBJECT.LOGIN.PANNEL", (_uint)LEVEL_ID::LEVEL_LOGIN, L"LOGIIN_PANNEL", &tDesc);

	
	
	

#pragma endregion 로그인 패널 












	return S_OK;
}

HRESULT CLoader::Loading_LevelLoby()
{
	return S_OK;
}

HRESULT CLoader::Loading_LevelGamePlay()
{
	return S_OK;
}
#ifdef _DEBUG
HRESULT CLoader::Loading_LevelMapTool()
{

	/*Add Texture*/
	/*로그인 화면*/
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.UI_LOGIN",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Login/Login.png"),1)
	);
	/*Text 유아이 만들자*/
	/*Button UI*/

	/*Room 화면*/

#pragma region 방
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.PLAYER_SELECT",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/SelectButton%d.png"), 4)
	);

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.SELECT_BG",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/SelecButtonBG.png"), 1)
	);

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.START_BUTTON",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/StartButton.png"), 1)
	);
	/*ShowCharacter*/
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.ROOM",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/Room.png"), 1)
	);

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.PLAYER_FRAME",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/PlayerFrame%d.png"), 2)
	);
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.PLAYER_IN_FRAME",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Players/Room/Player%d.png"), 4)
	);
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.PLAYER_BG",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/PlayerBG.png"), 1)
	);

	/*Chatting*/

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.CHAT_BG",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/ChattingBG.png"), 1)
	);

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_MAPTOOL,
		L"PROTO.TEXTURE.CHAT_SCROLL",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/ChattingScroll.png"), 1)
	);
#pragma endregion 방
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.LV_LOGIN.TESTOBJECT", CTestGameObject::Create(m_pDevice, m_pContext));
	CTestGameObject::TEST_DESC tDesc = {};
	tDesc.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc.fSpeedPerSec = 10.f;
	tDesc.ObjectPosition = { 0.f,0.f,1.f };
	tDesc.ObjectSize = { g_iWinSizeX,g_iWinSizeY,0.f };
	tDesc._ZOrder = 0;
	tDesc._bSelected = false;
	tDesc._UIMode = UIMODE::UI_FIXED;
	tDesc._UIState = UISTATE::UI_RELEASED;
	/*Add GameObject To Layer */
	m_pGameInstance->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.LV_LOGIN.TESTOBJECT", (_uint)LEVEL_ID::LEVEL_MAPTOOL, L"TestObject", &tDesc);




	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.OBJECT.TOOLOBJECT", CToolObject::Create(m_pDevice, m_pContext));
	/*CTestGameObject::TEST_DESC tDesc = {};
	tDesc.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc.fSpeedPerSec = 10.f;
	tDesc.ObjectPosition = { 0.f,0.f,1.f };
	tDesc.ObjectSize = { 100,100,0.f };
	tDesc._ZOrder = 0;
	tDesc._bSelected = false;
	tDesc._UIMode = UIMODE::UI_MOVABLE;
	tDesc._UIState = UISTATE::UI_RELEASED;*/
	/*Add GameObject To Layer */
	//m_pGameInstance->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.OBJECT.TOOLOBJECT",(_uint)LEVEL_ID::LEVEL_MAPTOOL, L"LAYER.TOOLOBJ", &tDesc);

	return S_OK;
}
#endif
HRESULT CLoader::Loading_LevelRoom()
{
	/*텍스처*/
	/*Room 화면*/

#pragma region 방
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.PLAYER_SELECT",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/SelectButton%d.png"), 4)
	);

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.SELECT_BG",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/SelecButtonBG.png"), 1)
	);

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.START_BUTTON",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/StartButton.png"), 1)
	);
	/*ShowCharacter*/
	/*전체 배경*/
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.ROOM",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/Room.png"), 1)
	);
	/*플레이어 */
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.PLAYER_FRAME",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/PlayerFrame%d.png"), 2)
	);

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.PLAYER_BG",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/PlayerBG.png"), 1)
	);
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.PLAYER_IN_FRAME",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Players/Room/Player%d.png"), 4)
	);
	/*Chatting*/

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.CHAT_BG",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/ChattingBG.png"), 1)
	);

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_ROOM,
		L"PROTO.TEXTURE.CHAT_SCROLL",
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Resources/Textures/Image/Sprite2/Room/ChattingScroll.png"), 1)
	);


	/*객체*/
	/*전체 배경*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.BG", CRoomBG::Create(m_pDevice, m_pContext));

	CRoomBG::ROOMBG_DESC  tDesc = {};
	tDesc.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc.fSpeedPerSec = 10.f;
	tDesc.ObjectPosition = { 0.f,0.f,1.f };
	tDesc.ObjectSize = { g_iWinSizeX,g_iWinSizeY,0.f };
	tDesc._ZOrder = 0;
	tDesc._bSelected = false;
	tDesc._UIMode = UIMODE::UI_FIXED;
	tDesc._UIState = UISTATE::UI_RELEASED;
	/*Add GameObject To Layer */
	m_pGameInstance->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.BG", (_uint)LEVEL_ID::LEVEL_ROOM, L"ROOM_BACKGROUND", &tDesc);



	/*채팅박스*/

	/*프로토 등록*/
	/*인풋 박스*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.CHAT_INPUTBOX", CInputBox::Create(m_pDevice, m_pContext));
	/* 채팅 박스*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.CHAT_CHATTINGBOX", CChattingBox::Create(m_pDevice, m_pContext));
	/*  스크롤*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.CHAT_SCROLL", CScroll::Create(m_pDevice, m_pContext));
	/* 스크롤 영역 */
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.CHAT_SCROLL_Range", CScrollRange::Create(m_pDevice, m_pContext));
	
	/* 채팅 박스 패널*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.CHAT_BG", CChattingBG::Create(m_pDevice, m_pContext));


	CChattingBG::CHATTING_DESC  tDesc1 = {};
	tDesc1.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc1.fSpeedPerSec = 10.f;
	tDesc1.ObjectPosition = { 0.f,0.f,1.f };
	tDesc1.ObjectSize = { g_iWinSizeX,g_iWinSizeY,0.f };
	tDesc1._ZOrder = 1;
	tDesc1._bSelected = false;
	tDesc1._UIMode = UIMODE::UI_FIXED;
	tDesc1._UIState = UISTATE::UI_RELEASED;
	/*Add GameObject To Layer */
	m_pGameInstance->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.CHAT_BG", (_uint)LEVEL_ID::LEVEL_ROOM, L"CHATTING_BACKGROUND", &tDesc1);

	/*Chatting PlayerBG*/

	/* 플레이어 프레임 */
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.PLAYER_FRAME", CPlayerFrame::Create(m_pDevice, m_pContext));

	/*플레이어 그림*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.PLAYER_IN_FRAME", CPlayerInFrame::Create(m_pDevice, m_pContext));
	/*플레이어 이름 */
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.PLAYER_NAME_IN_FRAME", CPlayerNameInFrame::Create(m_pDevice, m_pContext));
	
	/* 플레이어들 BG*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.PLAYER_BG", CPlayerBG::Create(m_pDevice, m_pContext));


	CPlayerBG::PLAYER_BG_DESC  tDesc2 = {};
	tDesc2.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc2.fSpeedPerSec = 10.f;
	tDesc2.ObjectPosition = { 0.f,0.f,1.f };
	tDesc2.ObjectSize = { g_iWinSizeX,g_iWinSizeY,0.f };
	tDesc2._ZOrder = 1;
	tDesc2._bSelected = false;
	tDesc2._UIMode = UIMODE::UI_FIXED;
	tDesc2._UIState = UISTATE::UI_RELEASED;
	/*Add GameObject To Layer */
	m_pGameInstance->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.PLAYER_BG", (_uint)LEVEL_ID::LEVEL_ROOM, L"PLAYERS_BACKGROUND", &tDesc2);

	/*플레이어 q*/
	/* 플레이어 프레임 */
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.SELECT_BUTTON", CSelectButton::Create(m_pDevice, m_pContext));

	/* 플레이어들 BG*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.SELECT_PANNEL", CSelectPannel::Create(m_pDevice, m_pContext));


	CSelectPannel::SELECTPANNEL_DESC  tDesc3 = {};
	tDesc3.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc3.fSpeedPerSec = 10.f;
	tDesc3.ObjectPosition = { 0.f,0.f,1.f };
	tDesc3.ObjectSize = { g_iWinSizeX,g_iWinSizeY,0.f };
	tDesc3._ZOrder = 1;
	tDesc3._bSelected = false;
	tDesc3._UIMode = UIMODE::UI_FIXED;
	tDesc3._UIState = UISTATE::UI_RELEASED;
	/*Add GameObject To Layer */
	m_pGameInstance->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.OBJECT.ROOM.SELECT_PANNEL",  (_uint)LEVEL_ID::LEVEL_ROOM, L"SELECT_PANNLE", &tDesc3);


	return S_OK;
}

HRESULT CLoader::Loading_LevelStatic()
{
	return S_OK;
}





CLoader* CLoader::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,LEVEL_ID NextLevel)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);
	if (pInstance->Initialize(NextLevel))
	{
		MSG_BOX("Failed To Create Loader");
		Safe_Delete(pInstance);
		return pInstance; 
	}
	return pInstance;

}
