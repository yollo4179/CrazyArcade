
#include "pch.h"
#include"MainApp.h"
#include"GameInstance.h"
#include"TestGameObject.h"
#include"LevelLoading.h"

#include"LevelMaptool.h"
#include"Prototype_Static.h"
#include"UIManager.h"
////#include"AbstractFactorySystem.h"
CMainApp::CMainApp()
	:m_pGameInstance{CGameInstance::GetInstance()}
{

	ENGINE_DESC EngineDesc = {};
	EngineDesc.hInstance = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iNumLevels= (_uint) LEVEL_ID::LEVEL_END;
	EngineDesc.isWindowed = true;
	EngineDesc.iViewportHeight = g_iWinSizeY;
	EngineDesc.iViewportWidth = g_iWinSizeX;

	m_pGameInstance->InitialzeEngine(EngineDesc, m_pDevice, m_pContext);

}
CMainApp::~CMainApp()
{
	/*UI Clear*/
	m_pGameInstance->ClearUIObjects();

	ClearAllLevel();
	m_pGameInstance->ClearLevelManager();
	ClearSingleton();
}
HRESULT CMainApp::Initialize()
{
	
	if (FAILED(m_pGameInstance->AddFont(L"BASE", L"../../EngineSDK/Font/zeldabold.spritefont")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->AddFont(L"INPUT", L"../../EngineSDK/Font/Medium20.spritefont")))
		return E_FAIL;
	ReadyProto_LEVELSTATIC();
	/*Level_Static*/
	//shared_ptr<CLevelLoading>ptr(CLevelLoading::Create(m_pDevice, m_pContext,LEVEL_ID::LEVEL_LOGIN));
	//shared_ptr<CLevelLoading>ptr(CLevelLoading::Create(m_pDevice, m_pContext,LEVEL_ID::LEVEL_MAPTOOL));
	shared_ptr<CLevelLoading>ptr(CLevelLoading::Create(m_pDevice, m_pContext, LEVEL_ID::LEVEL_LOGIN));
	m_pGameInstance->OpenLevel((_uint)LEVEL_ID::LEVEL_LOADING, ptr);
	
	
	
	
	return S_OK;
}

HRESULT CMainApp::ReadyProto_LEVELSTATIC()
{
	/*Buffer*/
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_STATIC,L"PROTO.BUFFER.RECT",CVIBufferRect::Create(m_pDevice,m_pContext));
	m_pGameInstance->AddProtoType((_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.RECTPOINT", CVIBufferPointRect::Create(m_pDevice, m_pContext));
	
	/*Shader*/
	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_STATIC,
		L"PROTO.SHADER.RECT",
		CShader::Create(m_pDevice, m_pContext, L"..\\Resources\\HLSL\\Rect.hlsl", VTXPOSTEX::Elements, VTXPOSTEX::iNumElements));

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_STATIC,
		L"PROTO.SHADER.RECTPOINT",
		CShader::Create(m_pDevice, m_pContext, L"..\\Resources\\HLSL\\PointInstanceRect.hlsl", VTXPOINTRECT::Elements, VTXPOINTRECT::iNumElements));

	m_pGameInstance->AddProtoType(
		(_uint)LEVEL_ID::LEVEL_STATIC,
		L"PROTO.BUFFER.DEBUGLINE",
		CDebugLine::Create(m_pDevice, m_pContext));


	
	return S_OK;
}


void CMainApp::Update(_float fTimeDelta)
{
	if (nullptr != m_pGameInstance)
		m_pGameInstance->Update_Engine(fTimeDelta);


}

void CMainApp::Render()
{
	m_pGameInstance->Render_Begin(_float4(0.f,1.f,0.f,0.f));
	m_pGameInstance->Render(); 
	m_pGameInstance->Render_End();
}

void CMainApp::ClearSingleton()
{
	CGameInstance::DestroyInstance();
}

void CMainApp::ClearAllLevel()
{
	if (nullptr != m_pGameInstance)
	{
		m_pGameInstance->ClearPrototypesOnAllLevels();
		m_pGameInstance->ClearObjectsOnAllLevels();
		/*\레벨이 인스턴스 가지고 있어서 레벨 박혀있으면 일단 해제시킨다.*/
		/*인스턴스 카운터를 1로 유지해야됨 */
		/*근데 Weak으로 참조하니까 또 괜찮네*/
		/*m_pGameInstance->ClearLevelManager();*/
	}
}

HRESULT CMainApp::SetUpEntryLevel(LEVEL_ID EntryLevel)
{
	
	
	m_pGameInstance->OpenLevel((_uint)EntryLevel, 
		shared_ptr<CLevelLoading>(
			CLevelLoading::Create(
				m_pDevice,
				m_pContext,
				EntryLevel))
		);
	return S_OK;

}

void CMainApp::RegisterAbstractFactoryDataAsset()
{
	//AstractFactory::Register<CToolObject,ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>>(L"TOOL_OBJECT",m_pDevice, m_pContext);
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp(); 
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Delete(pInstance);
		MSG_BOX("Failed To Create MainApp");
	}
	return pInstance;
}
