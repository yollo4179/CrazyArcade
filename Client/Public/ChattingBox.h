#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CChattingBox final : public CUIObject
{

public:
	typedef struct chattingBoxDesc:public CUIObject::UIOBJECT_DESC
	{

	}CHATTINGBOX_DESC;

private:
	CChattingBox(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CChattingBox(const CChattingBox& Prototype);
public:
	virtual ~CChattingBox() = default;
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

	TextSystem::TEXT_INPUT_STATE m_TextLogs= {};
	_bool InitOnce = { true  };
public:
	static shared_ptr<CChattingBox> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;
};
END
