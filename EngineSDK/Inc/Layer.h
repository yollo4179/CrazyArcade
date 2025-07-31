#pragma once
#include"Base.h"
BEGIN(Engine)
class CLayer final :public CBase
{
private:
	CLayer();
public:
	virtual ~CLayer(); 
	HRESULT Initialize(); 

	HRESULT Add_GameObject(CGameObject* pGameObject, const _wstring& strObjName = L"GameObject");
	HRESULT Add_GameObject(shared_ptr<CGameObject> pGameObject, const _wstring& strObjName = L"GameObject");

	void	PushObject(CGameObject* pObject , const _wstring& strObjName);
	void	PushObject(shared_ptr<CGameObject>& ObjectPtr, const _wstring& strObjName);
	HRESULT Delete_GameObject(const _wstring& strObjName);
	HRESULT Delete_GameObject(shared_ptr<CGameObject> pObj);
public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	shared_ptr<CGameObject> FindObjectByName(const _wstring);


public:
	

	typedef map<const _wstring, shared_ptr<CGameObject>> LAYER;
	LAYER m_pGameObjects;

public:
	static CLayer* Create();

};
END
