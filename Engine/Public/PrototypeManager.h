#pragma once
#include"Base.h"

BEGIN(Engine)
class CPrototypeManager final :public CBase
{
private:
	CPrototypeManager();
public:
	virtual ~CPrototypeManager();


public:
	HRESULT Initialize(_uint iNumLevels);

	HRESULT AddProtoType(_uint iProtoLevel, const _wstring strPrototypeTag, shared_ptr<CBase> pPrototype);
	HRESULT ErasePrototype(_uint iProtoLevel, const _wstring strPrototype);

	shared_ptr<CBase>FindPrototype(_uint iProtoLevel, const _wstring& strPrototypeTag);
	shared_ptr<CBase> Clone_Prototype(PROTOTYPE eType, _uint iProtoLevel, const _wstring& strPrototag, void* pArg);
	void ClearPrototypesOnLevel(_uint iProtoLevel);
	void ClearPrototypesOnAllLevels();


private:
	typedef map<const _wstring, shared_ptr<CBase> >  PROTOTYPES;
	weak_ptr<CGameInstance>m_pGameInstance = {};
	unique_ptr<PROTOTYPES[]> m_Prototypes = {};
	_uint m_iNumLevels = {};

public:
	static CPrototypeManager* Create(_uint iNumLevels);
};
END
