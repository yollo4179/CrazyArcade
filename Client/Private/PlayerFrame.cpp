#include"pch.h"
#include "PlayerFrame.h"
#include"GameInstance.h"
#include"SessionManager.h"
CPlayerFrame::CPlayerFrame(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{ pDevice ,pContext}
{
}

CPlayerFrame::CPlayerFrame(const CPlayerFrame& Prototype)
	:CUIObject{Prototype}
{
}

HRESULT CPlayerFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerFrame::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerFrame::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
	

	return S_OK;
}

void CPlayerFrame::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (-1 == m_SessionID)
		m_TexIndex = 1;
	else
		m_TexIndex = 0;

}

void CPlayerFrame::Update(_float fTimeDleta)
{
	__super::Update(fTimeDleta);
}

void CPlayerFrame::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayerFrame::Render()
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

HRESULT CPlayerFrame::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

shared_ptr<CPlayerFrame> CPlayerFrame::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CPlayerFrame> pInstance(new CPlayerFrame{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed ToCreate CPlayerFrame");
	}
	return pInstance;
}

shared_ptr<CGameObject> CPlayerFrame::Clone(void* pArg)
{
	shared_ptr<CPlayerFrame> pInstance(new CPlayerFrame{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CPlayerFrame");
	}
	return pInstance;
}
