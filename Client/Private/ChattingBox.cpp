#include"pch.h"
#include "ChattingBox.h"
#include"GameInstance.h"
#include"ChattingSocketManager.h"
#include"TextSystem.h"
CChattingBox::CChattingBox(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{pDevice, pContext}
{
}

CChattingBox::CChattingBox(const CChattingBox& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CChattingBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChattingBox::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChattingBox::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
	return S_OK;
}

void CChattingBox::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	if (InitOnce)
	{
		//위치 및 색상 설정 완료 
		m_TextLogs.Initialize(L"INPUT", XMLoadFloat4x4(&ComputeWorldMatrixAsChild()), 0.7f, Colors::White);
		InitOnce = false;  
	}
}


void CChattingBox::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CChattingBox::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CChattingBox::Render()
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
	
	pair<int, int> Indices = ChattingSocketManager::Get().GetDisplayIndex();
	m_TextLogs.Render_Texts(ChattingSocketManager::Get().GetChattingLogs(), Indices.first, Indices.second);
	return S_OK;
}

HRESULT CChattingBox::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

shared_ptr<CChattingBox> CChattingBox::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CChattingBox> pInstance(new CChattingBox{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype() ))
	{
		MSG_BOX("Failed ToCreate ChattingBox");
	}
	return pInstance; 

}

shared_ptr<CGameObject> CChattingBox::Clone(void* pArg)
{
	shared_ptr<CChattingBox> pInstance(new CChattingBox{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone ChattingBox");
	}
	return pInstance;
}
