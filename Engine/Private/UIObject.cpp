#include "UIObject.h"
#include"UIManager.h"
#include"GameInstance.h"
CUIObject::CUIObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CGameObject{pDevice, pContext}
{
}

CUIObject::CUIObject(const CUIObject& pObject)
	:CGameObject{pObject}
{
}

CUIObject::~CUIObject()
{
}

HRESULT CUIObject::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_bSelected = pDesc->_bSelected;
	m_ZOrder = pDesc->_ZOrder;
	m_UIMode = pDesc->_UIMode;
	m_UIState = pDesc->_UIState;
	return S_OK;
}

void CUIObject::MovementSelected()
{
#ifdef _DEBUG
	POINT m_CurMousePos = m_pGameInstance->GetMousePos();
	POINT m_Movement = { 
		m_CurMousePos.x - m_PreMousePos.x,
		-m_CurMousePos.y + m_PreMousePos.y
	};
	if (m_bSelected)
	{
		_vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
		vPos.m128_f32[0] += m_Movement.x; 
		vPos.m128_f32[1] += m_Movement.y;
		m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);

	}
	m_PreMousePos = m_CurMousePos;
#endif
}

void CUIObject::UpdateEvents()
{


	if (IsMouseInDomain())
	{

		if (MOUSE_DOWN(MOUSE::LB))
		{
			if (m_Functions[(_uint)UISTATE::UI_DOWN])
				m_Functions[(_uint)UISTATE::UI_DOWN]();
		}

		if (MOUSE_PRESSED(MOUSE::LB))
		{
			if (m_Functions[(_uint)UISTATE::UI_PRESSED])
				m_Functions[(_uint)UISTATE::UI_PRESSED]();
		}

		
		//OutOfBorder Event 
	}
	if (MOUSE_UP(MOUSE::LB))
	{
		if (m_Functions[(_uint)UISTATE::UI_UP])
			m_Functions[(_uint)UISTATE::UI_UP]();
	}
	if (MOUSE_RELEASED(MOUSE::LB))
	{
		if (m_Functions[(_uint)UISTATE::UI_RELEASED])
			m_Functions[(_uint)UISTATE::UI_RELEASED]();
	}
}
