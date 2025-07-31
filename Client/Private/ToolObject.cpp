#include"pch.h"
#include "ToolObject.h"
#include"GameInstance.h"
#ifdef _DEBUG
CToolObject::CToolObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CUIObject{pDevice,pContext}
{
}

CToolObject::CToolObject(const CToolObject& Prototype)
    :CUIObject{Prototype}
{
}

CToolObject::~CToolObject()
{
}

HRESULT CToolObject::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    ReadyProto();
    ReadyChildren();
    m_pGameInstance->SetToolMode(true);
    return S_OK;
}

HRESULT CToolObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CToolObject::ReadyProto()
{
    /*Todo Shader*/
    shared_ptr<CComponent> pComponent = {};
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.SHADER.RECTPOINT", nullptr));
    m_Components.emplace(L"SHADER.COM", pComponent);
    m_pShaderCom = dynamic_pointer_cast<CShader>(pComponent);

    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.RECTPOINT", nullptr));
    m_Components.emplace(L"BUFFER.COM", pComponent);
    m_pBufferCom = dynamic_pointer_cast<CVIBuffer>(pComponent);

    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.TEXTURE.UI_LOGIN", nullptr));
    m_Components.emplace(L"TEXTURE.COM", pComponent);
    m_pTextureCom = dynamic_pointer_cast<CTexture>(pComponent);

#ifdef _DEBUG
    pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
    m_Components.emplace(L"DEBUGLINE.COM", pComponent);
    m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(pComponent);
#endif

    return S_OK;
}

HRESULT CToolObject::ReadyChildren()
{
    return S_OK;
}

HRESULT CToolObject::BindBufferDesc()
{
    /*¹öÆÛ InputAssempber*/
        /*World matrix*/
    __super::BindBufferDesc();


    /*if (FAILED())
        return E_FAIL;*/


  

   /* if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom.get(), "g_WorldMatrix")))
        return E_FAIL;*/
    /*VP matrix*/
    

    return S_OK;
}

CToolObject::TOOLOBJECTDESC& CToolObject::GetTestObjectDesc()
{
    return m_TESTOBJECTDESC;
}

void CToolObject::SetTestObjectDesc(TOOLOBJECTDESC newDesc)
{
    m_TESTOBJECTDESC = newDesc;

    /*Transform Position*/
     m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&newDesc.ObjectPosition),1.f));
    /*Transform Size*/
     _vector vRight = XMVector3Normalize(m_pTransformCom->GetState(CTransform::STATE_RIGHT))*newDesc.ObjectSize.x;
     m_pTransformCom->SetState(CTransform::STATE_RIGHT, vRight );
     _vector vUp = XMVector3Normalize(m_pTransformCom->GetState(CTransform::STATE_UP)) * newDesc.ObjectSize.y;
     m_pTransformCom->SetState(CTransform::STATE_UP,vUp);


}

void CToolObject::SetTexture(shared_ptr<CTexture> pTexture)
{
    m_pTextureCom = pTexture;
}

void CToolObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CToolObject::Update(_float fTimeDelta)
{
    MovementSelected();
   /*_vector vLoc=  m_pTransformCom->GetState(CTransform::STATE_POSITION);
    XMStoreFloat3(&m_TESTOBJECTDESC.ObjectPosition, vLoc);*/

    __super::Update(fTimeDelta);
}

void CToolObject::Late_Update(_float fTimeDelta)
{
    shared_ptr<CGameObject> pShared = shared_from_this();
    m_pGameInstance->AddRenderObject(RENDER_UI, pShared);
  //  __super::Late_Update(fTimeDelta);
}

HRESULT CToolObject::Render()
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

shared_ptr<CToolObject> CToolObject::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    shared_ptr<CToolObject> pInstance(new CToolObject(pDevice, pContext));
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed_ to Create ToolObject");
        return nullptr; 
    }

    return pInstance;
}

shared_ptr<CGameObject> CToolObject::Clone(void* pArg)
{
    shared_ptr<CToolObject> pInstance(new CToolObject(*this));

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Clone Failed_ Tool Obj");
        return nullptr;
    }
    return pInstance;
}
#endif