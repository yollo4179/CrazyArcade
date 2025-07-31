#include"pch.h"
#include "PlayerBG.h"
#include"GameInstance.h"
#include"ObjectBuilder.h"
CPlayerBG::CPlayerBG(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{pDevice, pContext}
{
}

CPlayerBG::CPlayerBG(const CPlayerBG& Prototype)
	:CUIObject{Prototype}
{
}

HRESULT CPlayerBG::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerBG::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;
	ifstream In;
	try {

		In.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		In.open("../Resources/Json/Room_PlayerFrame.json");
		In >> jsonInfo;
		In.close();
	}
	catch (const std::ios_base::failure& e) {
		MSG_BOX("저장 에러 발생");
		e;
	}
	ObjectBuilderSystem::BuildObject(jsonInfo, shared_from_this());
	return S_OK;
}

HRESULT CPlayerBG::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
	return S_OK;
}

void CPlayerBG::Priority_Update(_float fTimeDelta)
{
	

	__super::Priority_Update(fTimeDelta);
}

void CPlayerBG::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CPlayerBG::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	shared_ptr<CGameObject> pShared = shared_from_this();
	m_pGameInstance->AddRenderObject(RENDER_UI, pShared);
}

HRESULT CPlayerBG::Render()
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

HRESULT CPlayerBG::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

shared_ptr<CPlayerBG> CPlayerBG::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CPlayerBG> pInstance(new CPlayerBG{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed ToCreate CPlayerBG");
	}
	return pInstance;
}

shared_ptr<CGameObject> CPlayerBG::Clone(void* pArg)
{
	shared_ptr<CPlayerBG> pInstance(new CPlayerBG{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CPlayerBG");
	}
	return pInstance;
}
