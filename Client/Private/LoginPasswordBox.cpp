#include"pch.h"
#include "LoginPasswordBox.h"
#include"GameInstance.h"

TextSystem::TEXT_INPUT_STATE  CLoginPasswordBox::TextInput;

CLoginPasswordBox::CLoginPasswordBox(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CUIObject{ pDevice, pContext }
{
}

CLoginPasswordBox::CLoginPasswordBox(const CLoginPasswordBox& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CLoginPasswordBox::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLoginPasswordBox::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    if (FAILED(ReadyProto()))
        return E_FAIL;

   
    return S_OK;
}

HRESULT CLoginPasswordBox::ReadyProto()
{
    shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
    m_Components.emplace(L"DEBUGLINE.COM", pComponent);
    m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
    return S_OK;
}

void CLoginPasswordBox::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    if (InitOnce) {
        TextInput.Initialize(L"BASE", XMLoadFloat4x4(&ComputeWorldMatrixAsChild()), 0.5f);
        InitOnce = false;
    }
    

    if (GetSelected())
    {
        if (g_Text != &TextInput)
            g_Text = &TextInput;
        TextInput.UpdateInput(fTimeDelta);
        TextInput.SelectStateIs(true);
    }
    else if(g_Text == GetTextState())
    {
        g_Text = nullptr;

        TextInput.SelectStateIs(false);
    }

    
}

void CLoginPasswordBox::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CLoginPasswordBox::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
   
    
}

HRESULT CLoginPasswordBox::Render()
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

    TextInput.Render_Text();
    return S_OK;
}

HRESULT CLoginPasswordBox::BindBufferDesc()
{
    __super::BindBufferDesc();
    return S_OK;
}

shared_ptr<CLoginPasswordBox> CLoginPasswordBox::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    shared_ptr<CLoginPasswordBox> pInstance ( new CLoginPasswordBox(pDevice, pContext));
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed_To Create CLoginPasswordBox");
    }

    return pInstance;
}

shared_ptr<CGameObject> CLoginPasswordBox::Clone(void* pArg)
{
    shared_ptr<CLoginPasswordBox> pInstance ( new CLoginPasswordBox(*this));
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed_To Clone CLoginPasswordBox");
       
    }
    return pInstance;
}
