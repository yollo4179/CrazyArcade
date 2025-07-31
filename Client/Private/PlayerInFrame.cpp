#include"pch.h"
#include "PlayerInFrame.h"
#include"GameInstance.h"
#include"SessionManager.h"
CPlayerInFrame::CPlayerInFrame(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CPlayerInFrame::CPlayerInFrame(const CPlayerInFrame& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CPlayerInFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerInFrame::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerInFrame::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif


	return S_OK;
}

void CPlayerInFrame::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	
	
		

	m_TexIndex = m_NowCharacter;
	
	m_TexIndex =std::clamp(m_TexIndex,(_int) 0, (_int)3);
}

void CPlayerInFrame::Update(_float fTimeDleta)
{
	__super::Update(fTimeDleta);
}

void CPlayerInFrame::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayerInFrame::Render()
{

	if (-1 != m_NowCharacter) {
	
		if (FAILED(BindBufferDesc()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pBufferCom->BindBufferDesc()))
			return E_FAIL;



		if (FAILED(m_pBufferCom->Render()))
			return E_FAIL;


		__super::Render();
	}
	return S_OK;
}

HRESULT CPlayerInFrame::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

shared_ptr<CPlayerInFrame> CPlayerInFrame::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CPlayerInFrame> pInstance(new CPlayerInFrame{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed ToCreate CPlayerInFrame");
	}
	return pInstance;
}

shared_ptr<CGameObject> CPlayerInFrame::Clone(void* pArg)
{
	shared_ptr<CPlayerInFrame> pInstance(new CPlayerInFrame{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CPlayerInFrame");
	}
	return pInstance;
}
