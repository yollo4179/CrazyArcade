#include "PrototypeManager.h"
#include"GameInstance.h"
#include"GameObject.h"
#include"Component.h"
CPrototypeManager::CPrototypeManager()
    :m_pGameInstance{CGameInstance::GetInstance()}
{
}
CPrototypeManager::~CPrototypeManager()
{
    int na=0;
}
CPrototypeManager* CPrototypeManager::Create(_uint iNumLevels)
{
    CPrototypeManager* pInstance = new CPrototypeManager();

    if (FAILED(pInstance->Initialize(iNumLevels)))
    {
        MSG_BOX("Failed To Make PrototypeManager");
        Safe_Delete(pInstance);
    }
    return pInstance;
}



HRESULT CPrototypeManager::Initialize(_uint iNumLevels)//LEVEL_ID :: LEVEL_END
{
   

    if (nullptr != m_Prototypes)return E_FAIL;

    m_Prototypes.reset(new PROTOTYPES[iNumLevels]); //(new PROTOTYPES[iNumLevels],std::default_delete<PROTOTYPES[]>());
   

    m_iNumLevels = iNumLevels;

    return S_OK;
}

HRESULT CPrototypeManager::AddProtoType(_uint iProtoLevel, const _wstring strPrototypeTag, shared_ptr<CBase> pPrototype)
{

    if (iProtoLevel >= m_iNumLevels || FindPrototype(iProtoLevel, strPrototypeTag))
    {
        return E_FAIL;
    }
    PROTOTYPES* pPrototypes = m_Prototypes.get();
    if (nullptr == pPrototypes) return E_FAIL;

    pPrototypes[iProtoLevel].emplace(strPrototypeTag, pPrototype);//알아서 생성자 호출 (복사 생성자 아님, 포인터 매개변수 생성자)
   
    return S_OK; 
    
}

HRESULT CPrototypeManager::ErasePrototype(_uint iProtoLevel, const _wstring strPrototypeTag)
{
    if (iProtoLevel >= m_iNumLevels || nullptr == FindPrototype(iProtoLevel, strPrototypeTag))
    {
        MSG_BOX("No Object Named That");
        return E_FAIL;
    }
    PROTOTYPES* pPrototypes = m_Prototypes.get();
    if (nullptr == pPrototypes) return E_FAIL;
    pPrototypes[iProtoLevel].erase(strPrototypeTag);


    return S_OK;
}

shared_ptr<CBase> CPrototypeManager::FindPrototype(_uint iProtoLevel, const _wstring& strPrototypeTag)
{

    if (iProtoLevel >= m_iNumLevels)return nullptr;

    PROTOTYPES* pPrototypes = m_Prototypes.get();
    if (nullptr == pPrototypes) return nullptr;

    auto iter = pPrototypes[iProtoLevel].find(strPrototypeTag);
    if (pPrototypes[iProtoLevel].end() == iter)
    {
       /* MSG_BOX("This Tag is Already used.");*/
        return nullptr;
    }

    return iter->second;
}

shared_ptr<CBase> CPrototypeManager::Clone_Prototype(PROTOTYPE eType, _uint iProtoLevel, const _wstring &strPrototag, void* pArg)
{
    /*등록한 원본 기준으로 사본 생성헤서 복제 */
    shared_ptr <CBase>m_pBase = {};
    m_pBase = FindPrototype(iProtoLevel, strPrototag);
    if (iProtoLevel >= m_iNumLevels || nullptr == m_pBase.get())
    {
        return nullptr; 
    }
    shared_ptr<CBase> pCopyObj = nullptr;
    switch (eType)
    {
    case PROTOTYPE::PROTO_OBJ :
        pCopyObj = dynamic_cast<CGameObject*>(m_pBase.get())->Clone(pArg);
        break;
    case PROTOTYPE::PROTO_COM :
        pCopyObj = dynamic_cast<CComponent*>(m_pBase.get())->Clone(pArg);
        
        break;
    }
    if (nullptr == pCopyObj)
    {
        //MSG_BOX("Failed To Clone in PrototypeManager");
    }

    return pCopyObj;
}

void CPrototypeManager::ClearPrototypesOnLevel(_uint iProtoLevel)
{
    if (iProtoLevel >=m_iNumLevels )
    {
        MSG_BOX("OutOf Bound : Num Levels in Proto Manager");
       return;
    }
    PROTOTYPES* pPrototypes = m_Prototypes.get();
    if (nullptr == pPrototypes) return ;

    for (auto& Prototype : pPrototypes[iProtoLevel])
    {
        Prototype.second.reset();//자동으로 ref 감소 
    }
    pPrototypes[iProtoLevel].clear();

}

void CPrototypeManager::ClearPrototypesOnAllLevels()
{
    
    PROTOTYPES* pPrototypes = m_Prototypes.get();
    if (nullptr == pPrototypes) return;

    for (size_t i = 0; i < m_iNumLevels; ++i) {
        for (auto& Prototype : pPrototypes[i])
        {
            Prototype.second.reset();//자동으로 ref 감소 
        }
        pPrototypes[i].clear();
    }

}
