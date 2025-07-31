#include"pch.h"
#include "SelectButton.h"
#include "GameInstance.h"
#include"SessionManager.h"
#include"ChattingSocketManager.h"
_uint CSelectButton::CharacterNumber = 0;

CSelectButton::CSelectButton(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{pDevice,pContext }
	, m_MyCharacterNumber{0}
{
}

CSelectButton::CSelectButton(const CSelectButton& Prototype)
	:CUIObject(Prototype)
	, m_MyCharacterNumber{ CharacterNumber++ }
{
}

HRESULT CSelectButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSelectButton::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSelectButton::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
	return S_OK;
}

void CSelectButton::Priority_Update(_float fTimeDelta)
{
	if (MOUSE_DOWN(MOUSE::LB))
	{
		if (IsMouseInDomain())
		{
			SessionManager::Get().MyCharacter = m_MyCharacterNumber;
			ChattingSocketManager::Get().SendNowInfo();

		}
	}

	__super::Priority_Update(fTimeDelta);
}

void CSelectButton::Update(_float fTimeDleta)
{
	__super::Update(fTimeDleta);
}

void CSelectButton::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSelectButton::Render()
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

HRESULT CSelectButton::BindBufferDesc()
{
	__super::BindBufferDesc();
	_bool IsEmpty = true;
	if (m_pTextureCom) {
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom.get(), "g_InputTexture", m_MyCharacterNumber)))
			return E_FAIL;
		IsEmpty = false;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Empty", &IsEmpty, sizeof(_bool))))
		return E_FAIL;
	return S_OK; 
}

shared_ptr<CSelectButton> CSelectButton::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CSelectButton> pInstance(new CSelectButton{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed ToCreate CSelectButton");
	}
	return pInstance;
}

shared_ptr<CGameObject> CSelectButton::Clone(void* pArg)
{
	shared_ptr<CSelectButton> pInstance(new CSelectButton{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CSelectButton");
	}
	return pInstance;
}
