#include"pch.h"
#include "TestGameObject.h"
#include"GameInstance.h"
shared_ptr<CTestGameObject> CTestGameObject::Create(const ComPtr<ID3D11Device>& pDevice,const ComPtr<ID3D11DeviceContext>& pContext)
{
	shared_ptr<CTestGameObject> pInstance (new CTestGameObject(pDevice,pContext));

	if(pInstance)
	{
		if(FAILED(pInstance->Initialize_Prototype()))
		{
			MSG_BOX("TestGameObject Create Failed");
			return nullptr;
		}
	}
	return pInstance;
}
shared_ptr<CGameObject> CTestGameObject::Clone(void* pArg)
{
	shared_ptr<CTestGameObject> pInstance ( new CTestGameObject(*this));

	if (pInstance)
	{
		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("CTestGameObject Create Failed");
			return nullptr;
		}
	}
	return pInstance;
}

CTestGameObject::CTestGameObject(const ComPtr<ID3D11Device>& pDevice,const ComPtr<ID3D11DeviceContext>& pContext)
	: CUIObject{ pDevice, pContext }
{

}
CTestGameObject::CTestGameObject(const CTestGameObject& rhs)
	: CUIObject{rhs}
{
}

HRESULT CTestGameObject::ReadyProto()
{
	/*Todo Shader*/
	shared_ptr<CComponent> pComponent = {};
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM,(_uint)LEVEL_ID::LEVEL_STATIC , L"PROTO.SHADER.RECTPOINT",nullptr)); 
	m_Components.emplace(L"SHADER.COM", pComponent);
	m_pShaderCom = dynamic_pointer_cast<CShader>(pComponent);

	pComponent = dynamic_pointer_cast<CComponent>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.RECTPOINT", nullptr));
	m_Components.emplace(L"BUFFER.COM", pComponent);
	m_pBufferCom = dynamic_pointer_cast<CVIBuffer>(pComponent);

	pComponent = dynamic_pointer_cast<CComponent>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.TEXTURE.ROOM", nullptr));
	m_Components.emplace(L"TEXTURE.COM", pComponent);
	m_pTextureCom = dynamic_pointer_cast<CTexture>(pComponent);

#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(pComponent);
#endif
	
	return S_OK;
}

HRESULT CTestGameObject::ReadyChildren()
{


	return S_OK;
}

HRESULT CTestGameObject::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

HRESULT CTestGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTestGameObject::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	
	if (FAILED(ReadyProto()))
		return E_FAIL; 
	if (FAILED(ReadyChildren()))
		return E_FAIL; 
	return S_OK;
}

void CTestGameObject::Priority_Update(_float fTimeDelta)
{
}

void CTestGameObject::Update(_float fTimeDelta)
{
	
}

void CTestGameObject::Late_Update(_float fTimeDelta)
{
	shared_ptr<CGameObject> pShared = shared_from_this();
	m_pGameInstance->AddRenderObject(RENDER_UI, pShared);
}

HRESULT CTestGameObject::Render()
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
	m_pDebugLineCom->DrawBox(DebugPos.m128_f32[0], DebugPos.m128_f32[1], DebugSize.x / 2.f, DebugSize.y / 2.f,_float4(1.f,0.f,0.f,1.f));
#endif

	return S_OK;
}

void CTestGameObject::Enable()
{
}

void CTestGameObject::Disalbe()
{
}
