#include"pch.h"
#include "PlayerNameInFrame.h"
#include"GameInstance.h"
#include"SessionManager.h"


CPlayerNameInFrame::CPlayerNameInFrame(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CUIObject{ pDevice, pContext }
{
}

CPlayerNameInFrame::CPlayerNameInFrame(const CPlayerNameInFrame& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CPlayerNameInFrame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerNameInFrame::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    if (FAILED(ReadyProto()))
        return E_FAIL;


    return S_OK;
}

HRESULT CPlayerNameInFrame::ReadyProto()
{
    shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
    m_Components.emplace(L"DEBUGLINE.COM", pComponent);
    m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
    return S_OK;
}

void CPlayerNameInFrame::Priority_Update(_float fTimeDelta)
{

    if (InitOnce) {
        TextInput.Initialize(L"INPUT", XMLoadFloat4x4(&ComputeWorldMatrixAsChild()), 0.7f);
        InitOnce = false;
        m_IsActive = false;
    }

   




    __super::Priority_Update(fTimeDelta);
}

void CPlayerNameInFrame::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}
void CPlayerNameInFrame::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);



}

HRESULT CPlayerNameInFrame::Render()
{
   /* if (FAILED(BindBufferDesc()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pBufferCom->BindBufferDesc()))
        return E_FAIL;



    if (FAILED(m_pBufferCom->Render()))
        return E_FAIL;*/

#ifdef _DEBUG
    _vector DebugPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    _float3 DebugSize = m_pTransformCom->ComputeScale();
    m_pDebugLineCom->DrawBox(DebugPos.m128_f32[0], DebugPos.m128_f32[1], DebugSize.x / 2.f, DebugSize.y / 2.f, _float4(1.f, 0.f, 0.f, 1.f));
#endif

    m_pParent;
    m_ChildeWorldMatrix;
  
    __super::Render();
    if (m_IsActive)
        TextInput.Render_Text(m_AccountName, Colors::Black);
    
    
    return S_OK;
}

HRESULT CPlayerNameInFrame::BindBufferDesc()
{
    __super::BindBufferDesc();
    return S_OK;
}

shared_ptr<CPlayerNameInFrame> CPlayerNameInFrame::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    shared_ptr<CPlayerNameInFrame> pInstance(new CPlayerNameInFrame(pDevice, pContext));
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed_To Create CPlayerNameInFrame");
    }

    return pInstance;
}

shared_ptr<CGameObject> CPlayerNameInFrame::Clone(void* pArg)
{
    shared_ptr<CPlayerNameInFrame> pInstance(new CPlayerNameInFrame(*this));
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed_To Clone CPlayerNameInFrame");
    }
    return pInstance;
}
