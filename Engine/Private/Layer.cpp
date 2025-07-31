#include "Layer.h"
#include"GameObject.h"
CLayer::CLayer()
{
    
}

CLayer::~CLayer()
{
}

HRESULT CLayer::Initialize()
{
    return S_OK;
}

/*프로토 타입에서 사본 만들어준다-> 이름이 세팅되어있다. 비교 상관없다.*/
HRESULT CLayer::Add_GameObject(CGameObject* pGameObject, const _wstring& strObjName)
{
    auto iter = m_pGameObjects.find(strObjName);

    _wstring newName = {}; 
    _uint NameID = {};
    
    if (m_pGameObjects.end() != iter)
    { 
       PushObject(pGameObject, strObjName);
    }
    else
    {
        m_pGameObjects.emplace(strObjName, pGameObject);
        pGameObject->SetName(strObjName);
    }
    return S_OK;
}
HRESULT CLayer::Add_GameObject(shared_ptr<CGameObject> pGameObject, const _wstring& strObjName)
{
    auto iter = m_pGameObjects.find(strObjName);

    _wstring newName = {};
    _uint NameID = {};

    if (m_pGameObjects.end() != iter)
    {
        PushObject(pGameObject, strObjName);
    }
    else
    {
        m_pGameObjects.emplace(strObjName, pGameObject);
        pGameObject->SetName(strObjName);
    }
    return S_OK;
}

void CLayer::PushObject(CGameObject* pObject, const _wstring& strObjName)
{
    auto iter = m_pGameObjects.find(strObjName);
    _uint iNumbering = { 1 };
    _wstring strTag = L"";
    do
    {
        strTag = strObjName + L"_"  + to_wstring(iNumbering++);
        iter = m_pGameObjects.find(strTag);
    } while (iter != m_pGameObjects.end());
    shared_ptr<CGameObject> ObjectPtr; 
    ObjectPtr.reset(pObject);
    m_pGameObjects.emplace(strTag, ObjectPtr);

    ObjectPtr->SetName(strTag);
    

}

void CLayer::PushObject(shared_ptr<CGameObject>& ObjectPtr, const _wstring& strObjName)
{
    auto iter = m_pGameObjects.find(strObjName);
    _uint iNumbering = { 1 };
    _wstring strTag = L"";
    do
    {
        strTag = strObjName + L"_" + to_wstring(iNumbering++);
        iter = m_pGameObjects.find(strTag);
    } while (iter != m_pGameObjects.end());
  
    m_pGameObjects.emplace(strTag, ObjectPtr);

    ObjectPtr->SetName(strTag);
}

HRESULT CLayer::Delete_GameObject(const _wstring& strObjName)
{
    auto iter = m_pGameObjects.find(strObjName);

    if (iter == m_pGameObjects.end())
        return E_FAIL;
    iter->second.reset();
    m_pGameObjects.erase(iter);

    return S_OK;
}

HRESULT CLayer::Delete_GameObject(shared_ptr<CGameObject> pObj)
{
    auto iter = m_pGameObjects.begin();

    for (; iter != m_pGameObjects.begin(); ++iter)
    {
        if (iter->second.get() == pObj.get())
        {
            iter->second.reset();
            m_pGameObjects.erase(iter);

            return S_OK;
        }
    }
    return E_FAIL;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
    for (auto& pGameObject : m_pGameObjects)
    {
        pGameObject.second->Priority_Update(fTimeDelta);
    }
    return;
}

void CLayer::Update(_float fTimeDelta)
{
    for (auto& pGameObject : m_pGameObjects)
    {
        pGameObject.second->Update(fTimeDelta);
    }
    return;
}

void CLayer::Late_Update(_float fTimeDelta)
{
    for (auto& pGameObject : m_pGameObjects)
    {
        pGameObject.second->Late_Update(fTimeDelta);
    }
    return;
}

shared_ptr<CGameObject> CLayer::FindObjectByName(const _wstring ObjName)
{
    
    auto iter = m_pGameObjects.find(ObjName);

    if (m_pGameObjects.end() == iter)
    {
       // MSG_BOX("해당 이름의 오브젝트가 없습니다 .LAYER");
        return nullptr; 
    }
    
    return iter->second;


}

CLayer* CLayer::Create()
{
    CLayer* pInstance = new CLayer; 

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("레이어 생성 실패");
        Safe_Delete(pInstance);  
    }

    return pInstance;
}


