#include"../Public/GameInstance.h"
#include"TimerManager.h"
#include"GraphicDevice.h"
#include"PrototypeManager.h"
#include"ObjectManager.h"
#include"LevelManager.h"
#include"KeyManager.h"
#include"Renderer.h"
#include"PipeLine.h"
#include"UIManager.h"
#include"FontManager.h"
IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimerManager{nullptr}
	, m_pGrapicDevice{ nullptr}
	, m_pPrototypeManager{}
	, m_pObjectManager{}
	, m_pLevelManager{}
	, m_pKeyManager{}
	, m_pRenderer{}
{
}

HRESULT CGameInstance::InitialzeEngine(const ENGINE_DESC& EngineDesc, ComPtr<ID3D11Device>& ppDevice, ComPtr<ID3D11DeviceContext>& ppContext)
{

	m_WinSizeX = EngineDesc.iViewportWidth;
	m_WinSizeY = EngineDesc.iViewportHeight;
	/*For GraphicDevice*/
	m_pGrapicDevice.reset(
		CGraphicDevice::Create(
			EngineDesc.hWnd,
			EngineDesc.isWindowed,
			EngineDesc.iViewportWidth,
			EngineDesc.iViewportHeight,
			&m_pDevice,
			&m_pContext
	));
	ppDevice= m_pDevice;
	ppContext = m_pContext;
	/*For KeyManager*/

	m_pKeyManager= shared_ptr<CKeyManager>(CKeyManager::Create(EngineDesc.hInstance,EngineDesc.hWnd,_float2(EngineDesc.iViewportWidth,EngineDesc.iViewportHeight )));
	/*ForTimer*/
	m_pTimerManager.reset(CTimerManager::Create()); 
	/*For Prototype Manager*/
	m_pPrototypeManager = shared_ptr<CPrototypeManager>(CPrototypeManager::Create(EngineDesc.iNumLevels));
	/*For OPbjectManager*/
	m_pObjectManager = shared_ptr<CObjectManager>(CObjectManager::Create(EngineDesc.iNumLevels));

	///*For LevelManager*/
	m_pLevelManager =shared_ptr< CLevelManager>(CLevelManager::Create());
	
	m_pRenderer = shared_ptr<CRenderer>(CRenderer::Create(m_pDevice, m_pContext));

	m_pPipeLine = shared_ptr<CPipeLine>(CPipeLine::Create(m_pDevice, m_pContext));

	m_pUIManager = shared_ptr<CUIManager>(CUIManager::Create());

	m_pFontManager= shared_ptr<CFontManager>(CFontManager::Create(m_pDevice, m_pContext));


#ifdef _DEBUG
	m_pImguiManager = shared_ptr<CImguiManager>( CImguiManager::Create( m_pDevice, m_pContext,EngineDesc.hWnd) );
#endif

	return S_OK;
}
static _float ftime;
void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pUIManager->Update(fTimeDelta);
	m_pKeyManager->Update();//입력받고 아래서 정보 처리 
	static _float ftime = fTimeDelta;

#ifdef _DEBUG
	m_pImguiManager->Update();
#endif

	m_pPipeLine->Update();
	m_pObjectManager->Priority_Update(fTimeDelta);

	m_pObjectManager->Update(fTimeDelta);

	m_pObjectManager->Late_Update(fTimeDelta);

	
	/*Renderer*/
	/*Level Manager*/
	m_pLevelManager->Update(ftime);
}

wstring CGameInstance::Mbcs2Wbcs(string str)
{
	return  wstring(str.begin(), str.end()); 	
}

string CGameInstance::Wbcs2Mbcs(wstring str)
{
	return string(str.begin(), str.end());
}

HRESULT CGameInstance::Render_Begin(_float4 ClearColor)
{
	//벡버퍼 클리어
	if (FAILED(m_pGrapicDevice->STEP1_1_ClearBackBufferView(ClearColor)))
		return E_FAIL; 
	
	if (FAILED(m_pGrapicDevice->STEP1_2_ClearDepthStencilView()))
		return E_FAIL; 
	return S_OK; 

}

HRESULT CGameInstance::Render_End()
{
	return m_pGrapicDevice->STEP2_Present();//그린거 보여준 후 교체 
}

HRESULT CGameInstance::Render()
{

	m_pRenderer->DrawObjects();
	
#ifdef _DEBUG
	m_pImguiManager->Render();
#endif
	
	/*Renderer*/
	return S_OK;
}

void CGameInstance::UpdateTimer(const _wstring& strTimerTag)
{
	m_pTimerManager->UpdateTimeDelta(strTimerTag);
}

CTimer* CGameInstance::FindTimer(const _wstring& strTimerTag)
{
	return m_pTimerManager->FindTimer(strTimerTag);
}

_float CGameInstance::GetTimeDelta(const _wstring& strTimerTag)
{
	return m_pTimerManager->GetTimeDelta(strTimerTag);
}

HRESULT CGameInstance::AddTimer(const _wstring& strTimerTag)
{
	return m_pTimerManager->AddTimer(strTimerTag);
}

HRESULT CGameInstance::AddProtoType(_uint iProtoLevel, const _wstring strPrototypeTag, shared_ptr<CBase> pPrototype)
{
	return m_pPrototypeManager->AddProtoType(iProtoLevel, strPrototypeTag, pPrototype);
}

HRESULT CGameInstance::ErasePrototype(_uint iLevel, const _wstring strPrototype)
{
	return m_pPrototypeManager->ErasePrototype(iLevel, strPrototype);
}

shared_ptr<CBase> CGameInstance::FindPrototype(_uint iLevel, const _wstring& strPrototypeTag)
{
	return m_pPrototypeManager->FindPrototype(iLevel, strPrototypeTag);
}

shared_ptr<CBase> CGameInstance::Clone_Prototype(PROTOTYPE eType, _uint iLevel, const _wstring strPrototag, void* pArg)
{
	return m_pPrototypeManager->Clone_Prototype(eType, iLevel, strPrototag, pArg);
}

void CGameInstance::ClearPrototypesOnLevel(_uint iLevel)
{
	m_pPrototypeManager->ClearPrototypesOnLevel(iLevel);
}

void CGameInstance::ClearPrototypesOnAllLevels()
{
	m_pPrototypeManager->ClearPrototypesOnAllLevels();

}

HRESULT CGameInstance::AddCopyObjectToLayer(_uint iPrototypeLevel, const _wstring& PrototypeTag, _uint iLayerLevel, const _wstring& strLayerTag, void* pArg, shared_ptr<CGameObject>* m_pGameObject)
{
	return m_pObjectManager->AddCopyObjectToLayer(iPrototypeLevel, PrototypeTag, iLayerLevel, strLayerTag, pArg, m_pGameObject);
}

HRESULT CGameInstance::AddCopyObjectToLayer(_uint iPrototypeLevel, const _wstring& PrototypeTag, _uint iLayerLevel, const _wstring& strLayerTag, shared_ptr<CGameObject> GameObjectPtr)
{
	return m_pObjectManager->AddCopyObjectToLayer(iPrototypeLevel, PrototypeTag, iLayerLevel, strLayerTag, GameObjectPtr);
}

HRESULT CGameInstance::AddGameObjectToLayer(_uint iLayerLevel, const _wstring& strLayerTag, CGameObject* pObject)
{
	return m_pObjectManager->AddGameObjectToLayer(iLayerLevel, strLayerTag, pObject);
}

HRESULT CGameInstance::DeleteGameObjectFromLayer(_uint iLayerLevel, const _wstring& strLayerTag, shared_ptr<CGameObject> m_pGameObject)
{
	return m_pObjectManager->DeleteGameObjectFromLayer( iLayerLevel, strLayerTag,  m_pGameObject);
}

HRESULT CGameInstance::DeleteGameObjectFromLayer(_uint iLayerLevel, const _wstring& strLayerTag, const _wstring ObjectName)
{
	return m_pObjectManager->DeleteGameObjectFromLayer( iLayerLevel,  strLayerTag,  ObjectName);
}

shared_ptr<CGameObject> CGameInstance::FindObjectByName(_uint iLayerLevel, const _wstring& strLayerTag, const _wstring ObjectName)
{
	return m_pObjectManager->FindObjectByName(iLayerLevel, strLayerTag, ObjectName);
}

void CGameInstance::ClearObjectsOnLevel(_uint iLevelIndex)
{
	m_pObjectManager->ClearObjectsOnLevel(iLevelIndex);
}

void CGameInstance::ClearObjectsOnAllLevels()
{
	m_pObjectManager->ClearObjectsOnAllLevels();
}

HRESULT CGameInstance::OpenLevel(_int iNewLevelID,const shared_ptr<CLevel>& pNewLevel)
{
	return m_pLevelManager->OpenLevel(iNewLevelID, pNewLevel);
}

void CGameInstance::ClearLevelManager()
{
	m_pLevelManager->ClearLevelManager();
}

_uint CGameInstance::GetNowLevel()
{
	return m_pLevelManager->GetNowLevel();
}

INPUT_STATE CGameInstance::GetKeyState(KEY eKey)
{
	return m_pKeyManager->GetKeyState(eKey);
	// TODO: 여기에 return 문을 삽입합니다.
}

INPUT_STATE CGameInstance::GetMouseState(MOUSE eKey)
{
	return m_pKeyManager->GetMouseState(eKey);
	// TODO: 여기에 return 문을 삽입합니다.
}

_long CGameInstance::GetMouseDIInput(MOUSEMOVE eMouseMove)
{

	return m_pKeyManager->GetMouseDIInput(eMouseMove);
}

POINT CGameInstance::GetMousePos()
{
	return m_pKeyManager->GetMousePos();
}

_bool CGameInstance::CheckMousePos(RECT _tRect)
{
	return m_pKeyManager->CheckMousePos(_tRect);
}

HRESULT CGameInstance::AddRenderObject(_uint RenderGroup, shared_ptr<CGameObject>& pGameObject)
{
	return m_pRenderer->AddRenderObject(RenderGroup, pGameObject);
}

const _float4x4* CGameInstance::GetPipeLineMatrixPtr(PIPELINE_MATRICES _Matrix)
{
	return m_pPipeLine->GetPipeLineMatrixPtr(_Matrix);
}

const _float4x4* CGameInstance::GetInverseMatrixPtr(PIPELINE_MATRICES _Matrix)
{
	return m_pPipeLine->GetInverseMatrixPtr(_Matrix);
}

void CGameInstance::SetPipeLine(PIPELINE_MATRICES _MatrixToSet, _matrix _Matrix)
{
	m_pPipeLine->SetPipeLine(_MatrixToSet, _Matrix);
	return; 
}

void CGameInstance::ClearUIObjects()
{
	m_pUIManager->ClearUIObjects();
}

void CGameInstance::AddUIObject(_wstring UITag, shared_ptr<CUIObject> pUIObject)
{
	m_pUIManager->AddUIObject(UITag, pUIObject);
}

shared_ptr<CUIObject> CGameInstance::GetUIObjectByTag(_wstring UITag)
{
	return m_pUIManager->GetUIObjectByTag(UITag);
}

shared_ptr<CUIObject> CGameInstance::GetSelectedObject()
{
	return m_pUIManager->GetSelectedObject();
}

void CGameInstance::DeleteUIObjectByName(_wstring UITag)
{
	m_pUIManager->DeleteUIObjectByName(UITag);
}

void CGameInstance::SetToolMode(_bool Ret)
{
	m_pUIManager->SetToolMode(Ret);
}

HRESULT CGameInstance::AddFont(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFontManager->AddFont(strFontTag, pFontFilePath);
}

HRESULT CGameInstance::RenderFont(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	return m_pFontManager->RenderFont(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale);
}

shared_ptr<SpriteFont> CGameInstance::GetSpriteFont(const _wstring& strFontTag)
{
	return m_pFontManager->GetSpriteFont(strFontTag);
}

shared_ptr<SpriteBatch> CGameInstance::GetSpriteBatch(const _wstring& strFontTag)
{
	return m_pFontManager->GetSpriteBatch(strFontTag);
}


CGameInstance::~CGameInstance()
{
	
	///*m_pImguiManager.reset();
	//m_pTimerManager.reset();
	//m_pGrapicDevice.reset();
	//m_pPrototypeManager.reset();
	//m_pObjectManager.reset();
	//m_pLevelManager.reset();

	//m_pDevice.Reset();
	//m_pContext.Reset();*/
}