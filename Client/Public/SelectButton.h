#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CSelectButton final: public CUIObject
{

public:
	typedef struct SelectPannel_Desc :public CUIObject::UIOBJECT_DESC
	{

	}SELECTPANNEL_DESC;

private:
	CSelectButton(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CSelectButton(const CSelectButton& Prototype);
public:
	virtual ~CSelectButton() = default;
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
	static shared_ptr<CSelectButton> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;

private:
	static _uint CharacterNumber;
	const _uint m_MyCharacterNumber;
};
END
