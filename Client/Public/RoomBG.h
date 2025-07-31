#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CRoomBG final :public CUIObject
{
public:
	typedef struct RoomBG_Desc :public CUIObject::UIOBJECT_DESC
	{

	}ROOMBG_DESC;

private:
	CRoomBG(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContetx);
	CRoomBG(const CRoomBG& Prototype);
public:
	virtual ~CRoomBG() = default;
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
	static shared_ptr<CRoomBG> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;

};
END
