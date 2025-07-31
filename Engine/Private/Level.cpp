#include "Level.h"
#include"GameInstance.h"

CLevel::CLevel(const ComPtr<ID3D11Device>& pDevice,const ComPtr<ID3D11DeviceContext>& pContext)
    : m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
{
}

HRESULT CLevel::Initialize()
{
    return S_OK;
}

void CLevel::Update(_float fTimeDelta)
{
}
