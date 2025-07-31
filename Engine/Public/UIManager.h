#pragma once
#include"Base.h"
#include"UIObject.h"
BEGIN(Engine)
class ENGINE_DLL CUIManager final :public CBase
{
public:
	CUIManager(); 
public:
	virtual ~CUIManager()=default; 
	void Update(_float fTimeDelta); 

	void UpdateSetZOrder(); 
	void ClearUIObjects(); 
	void CheckSelectObject();

	void AddUIObject(_wstring UITag, shared_ptr<CUIObject> pUIObject);
	void DeleteUIObjectByName(_wstring UITag);
	shared_ptr<CUIObject> GetUIObjectByTag(_wstring UITag);
	
	shared_ptr<CUIObject> GetSelectedObject(); 
public:
	/*ForTool*/
	void SetToolMode(_bool Ret) { m_bToolMode = Ret; };

public:
	static CUIManager* Create();
	
private:
	map<_wstring,  shared_ptr<CUIObject> > m_UIs;
	weak_ptr<CGameInstance> m_pGameInstance;
	_uint m_ZOrderBoundary = { 10 };
	_uint m_NowZOrderID = { 10 };
	/*UI매니저가 들고 있거나 마우스가 가지고 있거나 ...*/
	shared_ptr<CUIObject> m_pSelectedObject = {};

	_bool m_bToolMode = { false };
	static CUIManager m_UIManager;
};
END
