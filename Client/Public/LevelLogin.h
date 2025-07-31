#pragma once
#include"Client_Defines.h"
#include"Level.h"
BEGIN(Client)
class CLevelLogin final : public CLevel
{
private:
	CLevelLogin(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
public:
	virtual ~CLevelLogin();

public:
	HRESULT Initialize();
	bool IsLogined() { return m_isLogined; }

public:
	void Update(float fTimeDelta)override;
public:
	static CLevelLogin* Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	wstring AccountName = {}; 
	_bool m_isLogined = { false };
};
END
