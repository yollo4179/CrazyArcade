#include"pch.h"
#include "RoomBG.h"
#include"GameInstance.h"

CRoomBG::CRoomBG(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CUIObject{ pDevice, pContext }
{
}

CRoomBG::CRoomBG(const CRoomBG& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CRoomBG::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRoomBG::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (FAILED(ReadyProto()))
        return E_FAIL;

    return S_OK;

}

HRESULT CRoomBG::ReadyProto()
{
    /*Todo Shader*/
    shared_ptr<CComponent> pComponent = {};
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.SHADER.RECTPOINT", nullptr));
    m_Components.emplace(L"SHADER.COM", pComponent);
    m_pShaderCom = dynamic_pointer_cast<CShader>(pComponent);

    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.RECTPOINT", nullptr));
    m_Components.emplace(L"BUFFER.COM", pComponent);
    m_pBufferCom = dynamic_pointer_cast<CVIBuffer>(pComponent);

    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_ROOM, L"PROTO.TEXTURE.ROOM", nullptr));
    m_Components.emplace(L"TEXTURE.COM", pComponent);
    m_pTextureCom = dynamic_pointer_cast<CTexture>(pComponent);

#ifdef _DEBUG
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
    m_Components.emplace(L"DEBUGLINE.COM", pComponent);
    m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(pComponent);
#endif
    return S_OK;

}

void CRoomBG::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CRoomBG::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CRoomBG::Late_Update(_float fTimeDelta)
{
    shared_ptr<CGameObject> pShared = shared_from_this();
    m_pGameInstance->AddRenderObject(RENDER_UI, pShared);
    __super::Late_Update(fTimeDelta);

}

HRESULT CRoomBG::Render()
{
    if (FAILED(BindBufferDesc()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pBufferCom->BindBufferDesc()))
        return E_FAIL;



    if (FAILED(m_pBufferCom->Render()))
        return E_FAIL;



    __super::Render();

    return S_OK;
}

HRESULT CRoomBG::BindBufferDesc()
{

    __super::BindBufferDesc();

    return S_OK;
}

shared_ptr<CRoomBG> CRoomBG::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    shared_ptr<CRoomBG> pInstance(new CRoomBG(pDevice, pContext));
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed_To Create CRoomBG");
    }

    return pInstance;
}

shared_ptr<CGameObject>CRoomBG::Clone(void* pArg)
{
    shared_ptr<CRoomBG> pInstance(new CRoomBG(*this));
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed_To Clone CRoomBG");
    }
    return pInstance;
}
