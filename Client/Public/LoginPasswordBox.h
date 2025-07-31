#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CLoginPasswordBox final :public CUIObject
{
public:
	typedef struct LoginPasswordBoxDesc :public CUIObject::UIOBJECT_DESC
	{

	}LOGIN_PASSWORDBOX_DESC;

private:
	CLoginPasswordBox(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContetx);
	CLoginPasswordBox(const CLoginPasswordBox& Prototype);
public:
	virtual ~CLoginPasswordBox() = default;

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

	static TextSystem::TEXT_INPUT_STATE* GetTextState() { return &TextInput; };

private:
	_bool InitOnce = {true};

	static  TextSystem::TEXT_INPUT_STATE TextInput;
public:
	static shared_ptr<CLoginPasswordBox> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;

};
END