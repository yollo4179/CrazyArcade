#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CPlayerBG final :public CUIObject
{

public:
	typedef struct PlayerBG_Desc :public CUIObject::UIOBJECT_DESC
	{

	}PLAYER_BG_DESC;

private:
	CPlayerBG(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CPlayerBG(const CPlayerBG& Prototype);
public:
	virtual ~CPlayerBG() = default;
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
	static shared_ptr<CPlayerBG> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;
};
END
