#pragma once


#include "Base.h"

BEGIN(Engine)
class CTimerManager final : public CBase
{
private:
	CTimerManager(void);
public:
	virtual ~CTimerManager(void) = default;

public:
	_float GetTimeDelta(const _wstring& strTimerTag);
	HRESULT	AddTimer(const _wstring& strTimerTag);
	class CTimer* FindTimer(const _wstring& strTimerTag);
	void UpdateTimeDelta(const _wstring& strTimerTag);
	

private:
	map<const _wstring, shared_ptr<class CTimer> >m_mapTimer;

	


public:
	static CTimerManager* Create();
	
};

END

