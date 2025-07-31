#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CPlayerNameInFrame final :public CUIObject
{
public:
	typedef struct CPlayerNameInFrameDesc :public CUIObject::UIOBJECT_DESC
	{

	}PLAYER_NAME_IN_FRAME_DESC;

private:
	CPlayerNameInFrame(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContetx);
	CPlayerNameInFrame(const CPlayerNameInFrame& Prototype);
public:
	virtual ~CPlayerNameInFrame() = default;
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
	virtual void Active()override {  m_IsActive = true; }
	virtual void SetAccountName(_wstring _AccountName) { m_AccountName = _AccountName; };
public:
	static shared_ptr<CPlayerNameInFrame> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;

	
private:
	_bool InitOnce = true;
private:
	TextSystem::TEXT_INPUT_STATE  TextInput;

	_wstring m_AccountName = {}; 
	

};
END