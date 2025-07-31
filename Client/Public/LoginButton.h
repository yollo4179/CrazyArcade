#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CLoginButton final :public CUIObject
{
public:
	typedef struct LoginButtonDesc :public CUIObject::UIOBJECT_DESC
	{

	}LOGIN_BUTTON_DESC;

private:
	CLoginButton(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContetx);
	CLoginButton(const CLoginButton& Prototype);
public:
	virtual ~CLoginButton() = default;
public:
	virtual HRESULT			Initialize_Prototype()override;
	virtual HRESULT			Initialize(void* pArg)override;

	virtual HRESULT			ReadyProto()override;
public:
	void Priority_Update(_float fTimeDelta)override;
	void Update(_float fTimeDleta)override;
	void Late_Update(_float fTimeDelta) override;
	HRESULT Render();

	HRESULT BindBufferDesc()override;
protected:
	virtual void BindEvents();
	void isClicked();
	void isReleased();
public:
	static _bool GetClicked() { return m_IsClicked; };
	
private:
	static _bool m_IsClicked;
	
public:
	static shared_ptr<CLoginButton> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;

};
END