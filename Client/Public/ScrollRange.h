#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CScrollRange final :public CUIObject
{
public:
	typedef struct ScrollRange_Desc :public CUIObject::UIOBJECT_DESC
	{

	}SCROLLRANGE_DESC;

private:
	CScrollRange(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CScrollRange(const CScrollRange& Prototype);
public:
	virtual ~CScrollRange() = default;
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
	static shared_ptr<CScrollRange> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;
};
END
