#include"pch.h"
#include "LevelLoading.h"
#include"Loader.h"
#include"GameInstance.h"
#include"Levels.h"
CLevelLoading::CLevelLoading(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,LEVEL_ID EntryLevel)
    :CLevel{pDevice,pContext},
    m_EntryLevel{ EntryLevel },
    m_pLoader{nullptr} 
{
}

CLevelLoading::~CLevelLoading()
{
}

HRESULT CLevelLoading::Initialize()
{
    /*Loader 생성, 스레드 돌릴 */
    m_pGameInstance.lock()->ClearUIObjects();
    m_pLoader = shared_ptr<CLoader>(
        CLoader::Create(m_pDevice, m_pContext,m_EntryLevel)
    );
    return S_OK;
}

void CLevelLoading::Update(_float fTimeDelta)
{
    shared_ptr<CLevel> pLevel = {};
    if(m_pLoader->IsFinished())
    {
        
        switch (m_EntryLevel)
        {
        case LEVEL_ID::LEVEL_LOGIN:
            pLevel = shared_ptr<CLevelLogin>(CLevelLogin::Create(m_pDevice, m_pContext));
            break;
        case LEVEL_ID::LEVEL_LOBY:
            pLevel = shared_ptr<CLevelLoby>(CLevelLoby::Create(m_pDevice, m_pContext));
            break;
        case LEVEL_ID::LEVEL_ROOM:
            pLevel = shared_ptr<CLevelRoom>(CLevelRoom::Create(m_pDevice, m_pContext));
            break;
        case LEVEL_ID::LEVEL_GAMEPLAY:
            pLevel = shared_ptr<CLevelLogin>(CLevelLogin::Create(m_pDevice, m_pContext));
            break;
#ifdef _DEBUG
        case LEVEL_ID::LEVEL_MAPTOOL:
            pLevel = shared_ptr<CLevelMapTool>(CLevelMapTool::Create(m_pDevice, m_pContext));
            break;
#endif
        default:
            break;
        
        }
        m_pGameInstance.lock()->OpenLevel((_uint)m_EntryLevel, pLevel);
        return;
    }
   
}


CLevelLoading* CLevelLoading::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, LEVEL_ID EntryLevel)
{
    CLevelLoading* pInstance = new CLevelLoading(pDevice, pContext, EntryLevel);
    if (pInstance->Initialize())
    {
        MSG_BOX("Failed To Create Loader");
        Safe_Delete(pInstance);
        return pInstance;
    }
    return pInstance;
}
