#include"pch.h"
#include "InputBox.h"
#include"GameInstance.h"
#include"ChattingSocketManager.h"
 TextSystem::TEXT_INPUT_STATE CInputBox::TextInput;
CInputBox::CInputBox(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{pDevice, pContext}
{
}

CInputBox::CInputBox(const CInputBox& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CInputBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInputBox::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInputBox::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
	return S_OK;
}

void CInputBox::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


	if (InitOnce) {
		
		TextInput.Initialize(L"INPUT", XMLoadFloat4x4(&ComputeWorldMatrixAsChild()), 0.7f, Colors::White);
		InitOnce = false;
		
	}


	if (GetSelected())
	{
		TextInput.SelectStateIs(true);
		if( &TextInput != g_Text)
		{
			g_Text = GetTextState();
		}
		TextInput.UpdateInput(fTimeDelta);
		if (KEY_DOWN(KEY::ENTER))
		{
			ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
			ChattingSocketManager::Get().SendInputString(TextInput.FullText);
			
			TextInput.ClearAllBuffer();
		}
	}
	else
	{
		TextInput.SelectStateIs(true);
	}
}

void CInputBox::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CInputBox::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CInputBox::Render()
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

	TextInput.Render_Text();

	return S_OK;
}

HRESULT CInputBox::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

shared_ptr<CInputBox> CInputBox::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CInputBox> pInstance(new CInputBox{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed ToCreate CInputBox");
	}
	return pInstance;
}

shared_ptr<CGameObject> CInputBox::Clone(void* pArg)
{
	shared_ptr<CInputBox> pInstance(new CInputBox{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone ChattingBox");
	}
	return pInstance;
}



