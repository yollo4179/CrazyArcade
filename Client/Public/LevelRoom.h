#pragma once
#include"Client_Defines.h"
#include"Level.h"
BEGIN(Client)
class CLevelRoom final : public CLevel
{
private:
	CLevelRoom(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
public:
	virtual ~CLevelRoom();

public:
	HRESULT Initialize();

public:
	void Update(float fTimeDelta)override;
public:
	static CLevelRoom* Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};
END