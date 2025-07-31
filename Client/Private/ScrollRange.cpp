#include"pch.h"
#include "ScrollRange.h"
#include"GameInstance.h"
CScrollRange::CScrollRange(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{pDevice, pContext}
{
}

CScrollRange::CScrollRange(const CScrollRange& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CScrollRange::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScrollRange::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;

	return S_OK;
}

HRESULT CScrollRange::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
	return S_OK; 
}

void CScrollRange::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CScrollRange::Update(_float fTimeDleta)
{
	__super::Update(fTimeDleta);
}

void CScrollRange::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CScrollRange::Render()
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

HRESULT CScrollRange::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

shared_ptr<CScrollRange> CScrollRange::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CScrollRange> pInstance(new CScrollRange{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed ToCreate CScrollRange");
	}
	return pInstance;
}

shared_ptr<CGameObject> CScrollRange::Clone(void* pArg)
{
	shared_ptr<CScrollRange> pInstance(new CScrollRange{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CScrollRange");
	}
	return pInstance;
}

