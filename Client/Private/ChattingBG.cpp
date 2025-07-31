#include"pch.h"
#include "ChattingBG.h"
#include"GameInstance.h"
#include"ObjectBuilder.h"
CChattingBG::CChattingBG(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{ pDevice,pContext }
{
}

CChattingBG::CChattingBG(const CChattingBG& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CChattingBG::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChattingBG::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;
	ifstream In;
	try {

		In.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		In.open("../Resources/Json/Room_Chatting.json");
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

HRESULT CChattingBG::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
	return S_OK;
}

void CChattingBG::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CChattingBG::Update(_float fTimeDleta)
{
	__super::Update(fTimeDleta);
}

void CChattingBG::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	shared_ptr<CGameObject> pShared = shared_from_this();
	m_pGameInstance->AddRenderObject(RENDER_UI, pShared);
}

HRESULT CChattingBG::Render()
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

HRESULT CChattingBG::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

shared_ptr<CChattingBG> CChattingBG::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CChattingBG> pInstance(new CChattingBG{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed ToCreate CChattingBG");
	}
	return pInstance;
}

shared_ptr<CGameObject> CChattingBG::Clone(void* pArg)
{
	shared_ptr<CChattingBG> pInstance(new CChattingBG{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CChattingBG");
	}
	return pInstance;
}





