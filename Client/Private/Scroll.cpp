#include"pch.h"
#include "Scroll.h"
#include"GameInstance.h"
#include"ChattingSocketManager.h"
CScroll::CScroll(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CScroll::CScroll(const CScroll& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CScroll::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScroll::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (FAILED(ReadyProto()))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CScroll::ReadyProto()
{
	shared_ptr<CComponent> pComponent = {};
#ifdef _DEBUG
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, (_uint)LEVEL_ID::LEVEL_STATIC, L"PROTO.BUFFER.DEBUGLINE", nullptr));
	m_Components.emplace(L"DEBUGLINE.COM", pComponent);
	m_pDebugLineCom = dynamic_pointer_cast<CDebugLine>(m_Components.find(L"DEBUGLINE.COM")->second);
#endif
	return S_OK;
}

void CScroll::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (!m_Init)
	{
		m_Init = true;

		_float Height = GetParent().lock()->GetChildWorldMatrix().r[1].m128_f32[1];

		_vector vNowPosition = m_pTransformCom->GetState(CTransform::STATE_POSITION);
		m_Upper = vNowPosition.m128_f32[1];
		m_Lower = m_Upper - Height;
		m_NowPosition = m_Lower;
		vNowPosition.m128_f32[1] = m_NowPosition;
		m_pTransformCom->SetState(CTransform::STATE_POSITION, vNowPosition);
	}
	
	


	_float LogSize = ChattingSocketManager::Get().GetLogSize();
	pair<int, int> Indices = {}; //ChattingSocketManager::Get().GetDisplayIndex();
	if (LogSize >= 7)
	{
		_float Movement = MOUSE_MOVE(MOUSEMOVE::Z);
		if (Movement)
		{
			_vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);

			m_NowPosition += Movement * fTimeDelta * Rate;
			/*Clamp*/
			if (m_Lower > m_NowPosition)
			{
				m_NowPosition = m_Lower;
			}
			else if (m_NowPosition > m_Upper)
			{
				m_NowPosition = m_Upper;
			}
			vPos.m128_f32[1] = m_NowPosition;
			m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);
			ComputeWorldMatrixAsChild();
			//엔터 누르면 현재 위치를 LOWER로
		}
		_float Ratio = (m_NowPosition -m_Upper)/ (m_Lower - m_Upper);  //->0 ==index 6,   ->1 ==index size()-1
		_uint _NowNo = (_uint)( 7 + Ratio * (LogSize-7) ) ;
		
		Indices.first = _NowNo -7;
		Indices.second = _NowNo;
		ChattingSocketManager::Get().SetIndex(Indices.first, Indices.second);

	}
}

void CScroll::Update(_float fTimeDleta)
{
	__super::Update(fTimeDleta);
}

void CScroll::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CScroll::Render()
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

HRESULT CScroll::BindBufferDesc()
{
	__super::BindBufferDesc();

	return S_OK;
}

shared_ptr<CScroll> CScroll::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CScroll> pInstance(new CScroll{ pDevice, pContext });

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed ToCreate CScroll");
	}
	return pInstance;
}

shared_ptr<CGameObject> CScroll::Clone(void* pArg)
{
	shared_ptr<CScroll> pInstance(new CScroll{ *this });

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CScroll");
	}
	return pInstance;
}

