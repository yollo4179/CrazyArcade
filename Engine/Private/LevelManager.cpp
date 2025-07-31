#include "LevelManager.h"
#include"Level.h"
#include"GameInstance.h"
#include"UIManager.h"
CLevelManager::CLevelManager()
	: m_pGameInstance(CGameInstance::GetInstance())
{
}

CLevelManager::~CLevelManager()
{
	int b=0;
}

HRESULT CLevelManager::Initialize()
{
	return S_OK;
}

void CLevelManager::Update(_float fTimeDelta)
{
	if (m_pCurrentLevel == nullptr)
	{
		return;
	}
	m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevelManager::OpenLevel(_int iNewLevelID, const shared_ptr<CLevel>& pNewLevel)
{
	if (nullptr != m_pCurrentLevel)
	{
		/*Reset*/
		m_pGameInstance.lock()->ClearPrototypesOnLevel(m_iCurrentLevelID);
		m_pGameInstance.lock()->ClearObjectsOnLevel(m_iCurrentLevelID);
	}
	
	/*Set*/
	m_pCurrentLevel = pNewLevel;
	m_iCurrentLevelID = iNewLevelID;

	return S_OK;
}

void CLevelManager::ClearLevelManager()
{
	m_pCurrentLevel.reset();
}

CLevelManager* CLevelManager::Create()
{
	CLevelManager* pInstance = new CLevelManager;
	if(FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CLevelManager Create Failed");
		Safe_Delete(pInstance);
		return nullptr;
	}

	return pInstance;
}
