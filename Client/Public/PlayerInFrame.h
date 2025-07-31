#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CPlayerInFrame final : public CUIObject
{

public:
	typedef struct PlayerFrame_Desc :public CUIObject::UIOBJECT_DESC
	{

	}PLAYERFRAME_DESC;

private:
	CPlayerInFrame(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CPlayerInFrame(const CPlayerInFrame& Prototype);
public:
	virtual ~CPlayerInFrame() = default;
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
	void SetCharacter(_uint _Character) { m_NowCharacter = _Character; };

	_int m_NowCharacter = {-1};
private:
	shared_ptr<CTexture> m_pCharacterTexCom = {};
public:
	static shared_ptr<CPlayerInFrame> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;

};
END
