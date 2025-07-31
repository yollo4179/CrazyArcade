#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
#ifdef _DEBUG
BEGIN(Client)
class CToolObject :public CUIObject
{
public:
	typedef struct ToolObjectDesc:public CUIObject::UIOBJECT_DESC
	{

	}TOOLOBJECTDESC;

private:
	CToolObject(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
	CToolObject(const CToolObject& Prototype);
public:
	virtual ~CToolObject();

public:
	HRESULT Initialize(void* pArg);
	HRESULT Initialize_Prototype();
private:
	HRESULT ReadyProto()override;
	HRESULT ReadyChildren()override;

	HRESULT BindBufferDesc()override;
public:
	TOOLOBJECTDESC& GetTestObjectDesc();
	void SetTestObjectDesc(TOOLOBJECTDESC newDesc);
	void SetTexture(shared_ptr<CTexture> pTexture);




public:
	void Priority_Update(_float fTimeDelta) override;
	void Update(_float fTimeDelta) override;
	void Late_Update(_float fTimeDelta) override;

	HRESULT Render() override;


public:
	TOOLOBJECTDESC  m_TESTOBJECTDESC{ };


public:
	static shared_ptr<CToolObject> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext>pContext);
	shared_ptr<CGameObject> Clone(void * pArg);
};
END

#endif