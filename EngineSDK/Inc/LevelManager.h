#pragma once
#include"Base.h"
BEGIN(Engine)
class CLevelManager final : public CBase
{
private:
	CLevelManager(); 

public:
	virtual ~CLevelManager();

	HRESULT Initialize();
public:
	void Update(_float fTimeDelta);
	HRESULT OpenLevel(_int iNewLevelID,const  shared_ptr<CLevel>&pNewLevel);
	void ClearLevelManager();
	_uint GetNowLevel() { return m_iCurrentLevelID; };
#ifdef _DEBUG
	HRESULT Render() {};
#endif
public:
	static CLevelManager* Create();

	_int m_iCurrentLevelID = { 0 };
	shared_ptr<CLevel> m_pCurrentLevel = { nullptr };
	weak_ptr<CGameInstance> m_pGameInstance = {};
};
END
