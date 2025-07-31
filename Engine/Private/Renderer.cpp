#include "Renderer.h"
#include"GameObject.h"
#include"UIObject.h"
CRenderer::CRenderer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :m_pDevice{pDevice},
    m_pContext{pContext}
{
}

CRenderer::~CRenderer()
{
}

HRESULT CRenderer::Initialize()
{
    return S_OK;
}

HRESULT CRenderer::DrawObjects()
{
    if (FAILED(RenderUI()))
        return E_FAIL; 
    return S_OK;
}

HRESULT CRenderer::AddRenderObject(_uint RenderGroup, shared_ptr<CGameObject>& pGameObjectPtr)
{
    if (RenderGroup >= RENDER_END)
        return E_FAIL;
    m_pGameObjects[RenderGroup].push_back(pGameObjectPtr);
    return S_OK;
}

HRESULT CRenderer::RenderUI()
{
//#ifdef _DEBUG
    m_pGameObjects[RENDER_UI].sort(
        [](shared_ptr<CGameObject> p1, shared_ptr<CGameObject> p2) {
            return
                (static_cast<CUIObject*>( p1.get())->GetZOrder()
                    < static_cast<CUIObject*>( p2.get())->GetZOrder()
                    );
        }
    );
//#endif
    for (auto& Object : m_pGameObjects[RENDER_UI])
    {
        if(Object)
        Object->Render(); 

    }
    m_pGameObjects[RENDER_UI].clear(); 
    return S_OK;
}

CRenderer* CRenderer::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext>pContext)
{
    CRenderer* pInstance = new CRenderer(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Renderer ; 생성 실패 ");
        Safe_Delete(pInstance);
    }
    return pInstance; 
}
