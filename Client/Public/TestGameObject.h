#pragma once
#include"Client_Defines.h"
#include"UIObject.h"

BEGIN(Client)
class CTestGameObject final : public CUIObject
{
public:
	typedef struct Test_Desc : public CUIObject::UIOBJECT_DESC
	{



	}TEST_DESC;
private:
	CTestGameObject(const ComPtr<ID3D11Device>& pDevice, const ComPtr<ID3D11DeviceContext>& pContext);
	CTestGameObject(const CTestGameObject& rhs);
public:
	virtual ~CTestGameObject()=default;
private:
	HRESULT ReadyProto()override; 
	HRESULT ReadyChildren()override;
private:
	HRESULT BindBufferDesc()override; 
public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg = nullptr) override;

	void Priority_Update(_float fTimeDelta) override;
	void Update(_float fTimeDelta) override;
	void Late_Update(_float fTimeDelta) override;

	HRESULT Render() override;

	void Enable() override;
	void Disalbe() override;

public:
	static shared_ptr<CTestGameObject> Create(const ComPtr<ID3D11Device>& pDevice,const ComPtr<ID3D11DeviceContext>& pContext);
	shared_ptr<CGameObject> Clone(void* pArg = nullptr) override;

};
END
