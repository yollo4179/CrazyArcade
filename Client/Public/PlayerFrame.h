#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CPlayerFrame final: public CUIObject
{

public:
	typedef struct PlayerFrame_Desc :public CUIObject::UIOBJECT_DESC
	{

	}PLAYERFRAME_DESC;

private:
	CPlayerFrame(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CPlayerFrame(const CPlayerFrame& Prototype);
public:
	virtual ~CPlayerFrame() = default;
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
	
public :
	void SetSessionID(_int _SessionID) { m_SessionID = _SessionID; };
private:
	shared_ptr<CTexture> m_pCharacterTexCom = {};
	_int m_SessionID = -1;



public:
	static shared_ptr<CPlayerFrame> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;

};
END
