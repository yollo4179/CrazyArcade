#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CChattingBG final : public CUIObject
{
public:
	typedef struct Chatting_Desc :public CUIObject::UIOBJECT_DESC
	{

	}CHATTING_DESC;

private:
	CChattingBG(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CChattingBG(const CChattingBG& Prototype);
public:
	virtual ~CChattingBG() = default;
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

public:
	static shared_ptr<CChattingBG> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;
};
END
