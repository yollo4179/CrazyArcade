#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CInputBox final : public CUIObject
{

public:
	typedef struct InputBoxDesc :public CUIObject::UIOBJECT_DESC
	{

	}INPUTBOX_DESC;

private:
	CInputBox(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CInputBox(const CInputBox& Prototype);
public:
	virtual ~CInputBox() = default;
public:
	virtual HRESULT			Initialize_Prototype()override;
	virtual HRESULT			Initialize(void* pArg)override;


	HRESULT ReadyProto();
public:
	void Priority_Update(_float fTimeDelta)override;
	void Update(_float fTimeDleta)override;
	void Late_Update(_float fTimeDelta) override;
	HRESULT Render();

	HRESULT BindBufferDesc()override;

	static TextSystem::TEXT_INPUT_STATE* GetTextState() { return &TextInput; };
private:
	static TextSystem::TEXT_INPUT_STATE TextInput;
	_bool InitOnce = { true };
public:
	static shared_ptr<CInputBox> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;


};
END
