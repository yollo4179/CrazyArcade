#pragma once
#include"Client_Defines.h"
#include"UIObject.h"
BEGIN(Client)
class CScroll final : public CUIObject
{
public:
	typedef struct ScrollDesc :public CUIObject::UIOBJECT_DESC
	{

	}SCROLL_DESC;

private:
	CScroll(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CScroll(const CScroll& Prototype);
public:
	virtual ~CScroll() = default;
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

private:
	
	_bool m_Init = false;
	float m_Upper = 0.f;
	float m_Lower = 1.f;
	float  m_NowPosition = 0.f;
	_float Rate = 100.f; 

public:
	static shared_ptr<CScroll> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
	shared_ptr<CGameObject> Clone(void* pArg)override;
};
END
