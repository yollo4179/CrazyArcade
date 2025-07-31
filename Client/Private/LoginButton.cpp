#include"pch.h"
#include "LoginButton.h"
#include"GameInstance.h"
bool CLoginButton::m_IsClicked = false;
CLoginButton::CLoginButton(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CUIObject{ pDevice, pContext }
{
}

CLoginButton::CLoginButton(const CLoginButton& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CLoginButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLoginButton::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    if (FAILED(ReadyProto()))
        return E_FAIL;

    BindEvents();

    return S_OK;
}

HRESULT CLoginButton::ReadyProto()
{
   shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
    m_Components.emplace(L"DEBUGLINE.COM", pComponent);
    m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
    return S_OK;
}

void CLoginButton::Priority_Update(_float fTimeDelta)
{


    
    __super::Priority_Update(fTimeDelta);

    __super::UpdateEvents(); 
  

}

void CLoginButton::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CLoginButton::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

   
    
}

HRESULT CLoginButton::Render()
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

HRESULT CLoginButton::BindBufferDesc()
{
    __super::BindBufferDesc();
    return S_OK;
}

void CLoginButton::BindEvents()
{
    m_Functions[(_uint)UISTATE::UI_DOWN] = bind(&CLoginButton::isClicked,this);

    m_Functions[(_uint)UISTATE::UI_RELEASED] = bind(&CLoginButton::isReleased, this);

}

void CLoginButton::isClicked()
{
        m_IsClicked = true;
}

void CLoginButton::isReleased()
{
        m_IsClicked = false;
}

shared_ptr<CLoginButton> CLoginButton::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
   shared_ptr< CLoginButton> pInstance ( new CLoginButton(pDevice, pContext));
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed_To Create CLoginButton");
    }

    return pInstance;
}

shared_ptr<CGameObject> CLoginButton::Clone(void* pArg)
{
    shared_ptr<CLoginButton> pInstance ( new CLoginButton(*this));
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed_To Clone CLoginButton");
    }
    return pInstance;
}
