#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CLoginPannel final :public CUIObject
{
public:
	typedef struct LoginPannel_Desc :public CUIObject::UIOBJECT_DESC
	{

	}LOGINPANNEL_DESC;
	

private:
	CLoginPannel(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContetx);
	CLoginPannel(const CLoginPannel& Prototype);
public:
	virtual ~CLoginPannel() = default;

public:
	virtual HRESULT			Initialize_Prototype()override;
	virtual HRESULT			Initialize(void* pArg)override;

	void SettingInitializeBeforeFirstUpdate()override;

	virtual HRESULT			ReadyProto()override;
public:
	void Priority_Update(_float fTimeDelta)override;
	void Update(_float fTimeDleta)override;
	void Late_Update(_float fTimeDelta) override;
	HRESULT Render();

	HRESULT BindBufferDesc()override;

public:
	static shared_ptr<CLoginPannel> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;

};
END