#include"pch.h"
#include "LoginBG.h"
#include"GameInstance.h"

CLoginBG::CLoginBG(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CUIObject{pDevice, pContext}
{
}

CLoginBG::CLoginBG(const CLoginBG& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CLoginBG::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLoginBG::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (FAILED(ReadyProto()))
        return E_FAIL;
 
    return S_OK;
    
}

HRESULT CLoginBG::ReadyProto()
{
    /*Todo Shader*/
    shared_ptr<CComponent> pComponent = {};
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.SHADER.RECTPOINT", nullptr));
    m_Components.emplace(L"SHADER.COM", pComponent);
    m_pShaderCom = dynamic_pointer_cast<CShader>(pComponent);

    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.RECTPOINT", nullptr));
    m_Components.emplace(L"BUFFER.COM", pComponent);
    m_pBufferCom = dynamic_pointer_cast<CVIBuffer>(pComponent);

    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_LOGIN, L"PROTO.TEXTURE.UI_LOGIN", nullptr));
    m_Components.emplace(L"TEXTURE.COM", pComponent);
    m_pTextureCom = dynamic_pointer_cast<CTexture>(pComponent);

#ifdef _DEBUG
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
    m_Components.emplace(L"DEBUGLINE.COM", pComponent);
    m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(pComponent);
#endif
    return S_OK;
   
}

void CLoginBG::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CLoginBG::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CLoginBG::Late_Update(_float fTimeDelta)
{
    shared_ptr<CGameObject> pShared = shared_from_this();
    m_pGameInstance->AddRenderObject(RENDER_UI, pShared);
    __super::Late_Update(fTimeDelta);

}

HRESULT CLoginBG::Render()
{
    if (FAILED(BindBufferDesc()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pBufferCom->BindBufferDesc()))
        return E_FAIL;



    if (FAILED(m_pBufferCom->Render()))
        return E_FAIL;

#ifdef _DEBUG
    _vector DebugPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    _float3 DebugSize = m_pTransformCom->ComputeScale();
    m_pDebugLineCom->DrawBox(DebugPos.m128_f32[0], DebugPos.m128_f32[1], DebugSize.x / 2.f, DebugSize.y / 2.f, _float4(1.f, 0.f, 0.f, 1.f));
#endif

    __super::Render();

    return S_OK;
}

HRESULT CLoginBG::BindBufferDesc()
{

    __super::BindBufferDesc();

    return S_OK;
}

shared_ptr<CLoginBG> CLoginBG::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    shared_ptr<CLoginBG> pInstance ( new CLoginBG(pDevice, pContext));
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed_To Create LoginBG");
    }

    return pInstance;
}

shared_ptr<CGameObject>CLoginBG::Clone(void* pArg)
{
    shared_ptr<CLoginBG> pInstance ( new CLoginBG(*this));
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed_To Clone LoginBG");
    }    
    return pInstance;
}
