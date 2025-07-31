#include "ObjectManager.h"
#include"Layer.h"
#include"GameInstance.h"
#include"GameObject.h"
#include"UIObject.h"
#include"UIManager.h"
CObjectManager::CObjectManager()
	:m_pGameInstance{CGameInstance::GetInstance()}
{
}

HRESULT CObjectManager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;
	m_pLayers.reset(new LAYERS[m_iNumLevels]);

	return S_OK;
}

void CObjectManager::Priority_Update(_float fTimeDelta)
{
	/*클라 : 입력값 전달*/
	LAYERS* pLayers = m_pLayers.get();
	if (!pLayers)return;
	for (size_t i = 0; i < m_iNumLevels; ++i) {
		
		for (auto& pair : pLayers[i])
		{
			pair.second->Priority_Update(fTimeDelta);
		}
	}
	return;
}

void CObjectManager::Update(_float fTimeDelta)
{

	/*클라: 패킷 수신*/
	LAYERS* pLayers = m_pLayers.get();
	if (!pLayers)return;
	for (size_t i = 0; i < m_iNumLevels; ++i) {

		for (auto& pair : pLayers[i])//or curent Level
		{
			pair.second->Update(fTimeDelta);
		}
	}
	return;
}


void CObjectManager::Late_Update(_float fTimeDelta)
{
	/*변화 적용*/
	LAYERS* pLayers = m_pLayers.get();
	if (!pLayers)return;
	for (size_t i = 0; i < m_iNumLevels; ++i) {

		for (auto& pair : pLayers[i])
		{
			pair.second->Late_Update(fTimeDelta);
		}
	}
	return;
	/*렌더*/
}

void CObjectManager::ClearObjectsOnLevel(_uint iLevelIndex)
{

	LAYERS* pLayers = m_pLayers.get();
	if (!pLayers)return;
	for (auto& pair : pLayers[iLevelIndex])
		{
			pair.second.reset();
		}
	pLayers[iLevelIndex].clear();
	return;
}

void CObjectManager::ClearObjectsOnAllLevels()
{
	LAYERS* pLayers = m_pLayers.get();
	if (!pLayers)return;
	for (size_t i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& pair : pLayers[i])
		{
			pair.second.reset();
		}
	}
	return;
}

HRESULT CObjectManager::AddCopyObjectToLayer(_uint iPrototypeLevel, const _wstring& PrototypeTag, _uint iLayerLevel, const _wstring& strLayerTag, void* pArg, shared_ptr<CGameObject>* GetNowPushedObject)
{
	if (iPrototypeLevel >= m_iNumLevels) return E_FAIL;

	

	shared_ptr<CGameObject> pGameObject = dynamic_pointer_cast<CGameObject>(m_pGameInstance.lock()->Clone_Prototype(PROTOTYPE::PROTO_OBJ, iPrototypeLevel, PrototypeTag, pArg));
	if (nullptr == pGameObject) { MSG_BOX("오브젝트 추가 실패"); return E_FAIL; };
	


	shared_ptr<CLayer> pLayerPtr = FindLayer(iLayerLevel, strLayerTag);
	if (nullptr == pLayerPtr.get())
	{
		/*레이어 없으면 레이어 추가*/
		CLayer* pLayer = CLayer::Create(); 
		pLayerPtr.reset(pLayer);

		LAYERS* pLayers = m_pLayers.get();
		pLayers[iLayerLevel].emplace(strLayerTag, pLayerPtr);
	}
	pLayerPtr->Add_GameObject(pGameObject, strLayerTag);

	/*UIObject라면*/
	if (nullptr != dynamic_cast<CUIObject*>(pGameObject.get())) {
		m_pGameInstance.lock()->
			AddUIObject(
				pGameObject->GetName(),
				dynamic_pointer_cast<CUIObject>(pGameObject->GetShared())
			);

	}
	if (GetNowPushedObject)
	{
		*GetNowPushedObject = pGameObject->GetShared();
	}
	return S_OK;
}

HRESULT CObjectManager::AddCopyObjectToLayer(_uint iPrototypeLevel, const _wstring& PrototypeTag, _uint iLayerLevel, const _wstring& strLayerTag, shared_ptr<CGameObject> GameObjectPtr)
{
	if (iPrototypeLevel >= m_iNumLevels) return E_FAIL;
	
	shared_ptr<CLayer> pLayerPtr = FindLayer(iLayerLevel, strLayerTag);
	if (nullptr == pLayerPtr.get())
	{
		/*레이어 없으면 레이어 추가*/
		CLayer* pLayer = CLayer::Create();
		pLayerPtr.reset(pLayer);

		LAYERS* pLayers = m_pLayers.get();
		pLayers[iLayerLevel].emplace(strLayerTag, pLayerPtr);
	}
	

	pLayerPtr->Add_GameObject(GameObjectPtr, strLayerTag);

	/*UIObject라면*/
	if (nullptr != dynamic_cast<CUIObject*>(GameObjectPtr.get())) {
		m_pGameInstance.lock()->
			AddUIObject(
				GameObjectPtr->GetName(),
				dynamic_pointer_cast<CUIObject>(GameObjectPtr)
			);

	}
	
	return S_OK;
}

HRESULT CObjectManager::AddGameObjectToLayer(_uint iLayerLevel, const _wstring& strLayerTag, CGameObject* pObject)
{

	if (nullptr == pObject)
	{
		MSG_BOX("오브젝트가 널 포인터"); return E_FAIL;
	}
	shared_ptr<CLayer> pLayerPtr = FindLayer(iLayerLevel, strLayerTag);
	if (nullptr == pLayerPtr.get())
	{
		/*레이어 없으면 레이어 추가*/
		CLayer* pLayer = CLayer::Create();
		pLayerPtr.reset(pLayer);

		LAYERS* pLayers = m_pLayers.get();
		pLayers[iLayerLevel].emplace(strLayerTag, pLayerPtr);
	}
	/*레이어에 오브젝트 추가*/
	if (pObject->GetName().size())
		pLayerPtr->Add_GameObject(pObject, pObject->GetName());
	else
		pLayerPtr->Add_GameObject(pObject);

	return S_OK;
}

HRESULT CObjectManager::DeleteGameObjectFromLayer(_uint iLayerLevel, const _wstring& strLayerTag, shared_ptr<CGameObject> m_pGameObject)
{
	/*todo  레이어 찾기 ->레이어에서 객체로 삭제 삭제*/
	weak_ptr<CLayer> pLayer  =FindLayer(iLayerLevel, strLayerTag);
	if (nullptr == pLayer.lock().get()) { return E_FAIL; };

	if(FAILED(pLayer.lock().get()->Delete_GameObject(m_pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjectManager::DeleteGameObjectFromLayer(_uint iLayerLevel, const _wstring& strLayerTag, const _wstring ObjectName)
{
	weak_ptr<CLayer> pLayer = FindLayer(iLayerLevel, strLayerTag);
	if (nullptr == pLayer.lock().get()) { return E_FAIL; };

	if (FAILED(pLayer.lock().get()->Delete_GameObject(ObjectName)))
		return E_FAIL;
	return S_OK;
}

shared_ptr<CGameObject> CObjectManager::FindObjectByName(_uint iLayerLevel, const _wstring& strLayerTag, const _wstring ObjectName)
{

	CLayer*pLayer =  FindLayer(iLayerLevel, strLayerTag).get();
	if (!pLayer) return nullptr;



	weak_ptr<CGameObject> pGameObject = pLayer->FindObjectByName(ObjectName);
	if (nullptr == pGameObject.lock())return nullptr; 

	return pGameObject.lock();
}

shared_ptr<CLayer> CObjectManager::FindLayer(_uint iLevel, const _wstring& strLayer)
{
	if (iLevel >= m_iNumLevels) {
		MSG_BOX("OutOfLevels: ObjManager"); return nullptr;
	}

	LAYERS* pLayers = m_pLayers.get();
	if (!pLayers) {
		MSG_BOX("NoLayers :ObjManager"); return nullptr;
	}

	auto iter  =  pLayers[iLevel].find(strLayer);
	if (pLayers[iLevel].end() == iter) return nullptr; 

	return  iter->second;
}

CObjectManager* CObjectManager::Create(_uint iNumLevels)
{
	CObjectManager* pInstance = new CObjectManager;
	if (FAILED(pInstance->Initialize(iNumLevels)))
	{

		MSG_BOX("게임 오브젝트 생성 실패");
		Safe_Delete(pInstance);
	}
	return pInstance;
}
