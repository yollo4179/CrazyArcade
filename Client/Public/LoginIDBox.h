#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CLoginIDBox final :public CUIObject
{
public:
	typedef struct LoginIDBoxDesc :public CUIObject::UIOBJECT_DESC
	{

	}LOGIN_IDBOX_DESC;

private:
	CLoginIDBox(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContetx);
	CLoginIDBox(const CLoginIDBox& Prototype);
public:
	virtual ~CLoginIDBox() = default;
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
public:
	/*util*/
	void KeyDown()
	{

	}
	static TextSystem::TEXT_INPUT_STATE* GetTextState(){ return &TextInput; };

public:
	static shared_ptr<CLoginIDBox> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;


private:
	_wstring IDText; 
	size_t CursorPos;//인덱스만 보관
	_bool OnCursorBlink;
	float BlinkPeriod = { 0.5f };

	_bool InitOnce;
private:
	  static TextSystem::TEXT_INPUT_STATE  TextInput;
	


};
END