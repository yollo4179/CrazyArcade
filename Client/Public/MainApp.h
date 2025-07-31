#pragma once
#include"Base.h"
#include"Client_Defines.h"
BEGIN(Client)
class CMainApp final :public CBase
{
private:
	CMainApp();
public:
	virtual ~CMainApp();

public:
	HRESULT Initialize();
public:
	HRESULT ReadyProto_LEVELSTATIC(); 

	
public:
	void Update(_float fTimeDelta);
	void Render();

	void ClearSingleton();
	void ClearAllLevel();
	HRESULT SetUpEntryLevel(LEVEL_ID EntryLevel);
	void RegisterAbstractFactoryDataAsset();
public:
	std::shared_ptr<CGameInstance> m_pGameInstance = {};
	ComPtr< ID3D11Device> m_pDevice = {};
	ComPtr< ID3D11DeviceContext> m_pContext= {};
public:
	static CMainApp* Create(); 
};
END
