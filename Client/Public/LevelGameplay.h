#pragma once
#include"Client_Defines.h"
#include"Level.h"
class CLevelGameplay final : public CLevel
{
private:
	CLevelGameplay(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
public:
	virtual ~CLevelGameplay(); 
public:
	HRESULT Initialize();
public:
	void Update(_float fTimeDelta)override;

	_bool m_GameEnd = {false};
public:
	static CLevelGameplay* Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);

};

