#pragma once
#include"Client_Defines.h"
#include"Level.h"
BEGIN(Client)
class CLevelLoby :public CLevel
{
private:
	CLevelLoby(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
public:
	virtual ~CLevelLoby();

public:
	HRESULT Initialize();

public:
	void Update(float fTimeDelta)override;
public:
	static CLevelLoby* Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	
};

END
