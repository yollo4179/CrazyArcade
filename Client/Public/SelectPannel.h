#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CSelectPannel final : public CUIObject
{
public:
	typedef struct SelectPannel_Desc :public CUIObject::UIOBJECT_DESC
	{

	}SELECTPANNEL_DESC;

private:
	CSelectPannel(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CSelectPannel(const CSelectPannel& Prototype);
public:
	virtual ~CSelectPannel() = default;
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
	static shared_ptr<CSelectPannel> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;


};
END
