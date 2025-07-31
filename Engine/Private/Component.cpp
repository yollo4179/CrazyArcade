#include "Component.h"
#include"GameInstance.h"
CComponent::CComponent(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:m_pDevice{pDevice},// shared 복사 생성자 -> Ref1증가 
	m_pContext{pContext},
	m_pGameInstance{ CGameInstance::GetInstance() }
{
}

CComponent::CComponent(const CComponent& Prototype)
	:m_pDevice{Prototype.m_pDevice},
	m_pContext{ Prototype.m_pContext },
	m_pGameInstance{ Prototype .m_pGameInstance}
{
}

HRESULT CComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Initialize(void* pArg)
{
	m_pOwner = static_cast<CComponent_DESC*>(pArg)->pOwner;

	return S_OK;
}




