#pragma once
#include"GameObject.h"
#include"TextSystem.h"
BEGIN(Engine)
enum class UIMODE
{
	UI_FIXED,
	UI_MOVABLE,
	UI_END,
};
enum class UISTATE
{
	UI_PRESSED,
	UI_DOWN,
	UI_UP,
	UI_RELEASED,
	UI_STATE_END
};

class ENGINE_DLL CUIObject abstract : public CGameObject 
{
	/*For Test UI*/
public:
	typedef struct UIDesc :CGameObject::OBJECT_DESC
	{
		_uint _ZOrder = {};
		_bool _bSelected = {};
		UIMODE	_UIMode = {};
		UISTATE _UIState= { UISTATE::UI_RELEASED };
	}UIOBJECT_DESC;
protected:
	CUIObject(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
	CUIObject(const CUIObject& pObject);
public:
	virtual ~CUIObject();

public:
	HRESULT Initialize(void* pArg)override;
public:
	_uint GetZOrder() { return m_ZOrder; };
	void SetZOrder(_uint newOrder) { m_ZOrder = newOrder;};

	_uint GetSelected() { return m_bSelected; };
	void SetSelected(_uint newSelected) { m_bSelected = newSelected; };

	UIMODE GetUIMode() { return m_UIMode; };
	void SetUIMode(UIMODE newMode) { m_UIMode = newMode; };

	void MovementSelected();
	//virtual TextSystem::TEXT_INPUT_STATE* GetTextState() { return nullptr; };
	
	
	

public:
#ifdef _DEBUG
	virtual void Inspector()override { __super::Inspector(); };
#endif
protected:
	virtual void UpdateEvents();
	virtual void BindEvents() {};
	/*Button Function*/
	std::function<void()> m_Functions[(_uint)UISTATE::UI_STATE_END] = {};

	POINT m_PreMousePos = {};
public:
	virtual shared_ptr<CGameObject> Clone(void* pArg) = 0;
private:
	_uint m_ZOrder = {};
	_bool m_bSelected = {};
	UIMODE	m_UIMode = {};
	UISTATE m_UIState = {};

	
	
};
END
