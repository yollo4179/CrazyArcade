#pragma once
#include"Client_Defines.h"
#include"Level.h"
BEGIN(Client)
class CLevelLoading final : public CLevel
{

private:
	CLevelLoading(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext>pContext, LEVEL_ID EntryLevel);
public:
	virtual ~CLevelLoading();
public:
	HRESULT Initialize();

	virtual void Update(_float fTimeDelta)override;


private:
	

	shared_ptr<class CLoader> m_pLoader = {};
	LEVEL_ID		m_EntryLevel = {};
	_bool IsLoaded;
public:
	static CLevelLoading* Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext>pContext,LEVEL_ID EntryLevel);

};
END
