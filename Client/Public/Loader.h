#pragma once
#include"Client_Defines.h"
#include"Base.h"
BEGIN(Client)
class CLoader final :public CBase
{
private:
	CLoader(ComPtr<ID3D11Device>m_pDevice, ComPtr<ID3D11DeviceContext>m_pContex);
public:
	virtual ~CLoader();
public:
	HRESULT Initialize(LEVEL_ID);
	_bool IsFinished() { return m_IsFinished; };

	HRESULT Loading();
	HRESULT Loading_LevelLogin();
	HRESULT Loading_LevelLoby();
	HRESULT Loading_LevelGamePlay();
#ifdef _DEBUG
	HRESULT Loading_LevelMapTool();
#endif
	HRESULT Loading_LevelRoom();
	HRESULT Loading_LevelStatic();
	CRITICAL_SECTION m_CriticalSection = {}; 
	
	LEVEL_ID m_iLevelId;
	HANDLE  m_hThread;
	_bool m_IsFinished = false;
	class CLoader* m_pLoader = {};
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext>m_pContext;
	shared_ptr<CGameInstance> m_pGameInstance = {};
public:
	static CLoader* Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext>pContext,LEVEL_ID NextLevel);
};
END
