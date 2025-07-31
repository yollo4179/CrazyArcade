#pragma once
#include"Base.h"
BEGIN(Engine)
class CTimer :public CBase
{
private:
	CTimer();
public:
	virtual ~CTimer(void)=default;


public:
	_float GetTimeDelta(void)const
	{
		return m_fTimeDelta; 
	}
public:
	HRESULT ReadyTimer(void);
	void UpdateTimer(void);

private: 
	//클럭 신호 
	LARGE_INTEGER			m_FrameTime = {};
	LARGE_INTEGER			m_OneSecAgoTime = {};
	LARGE_INTEGER			m_LastTime = {};
	LARGE_INTEGER			m_CpuTick = {};
	
	_float m_fTimeDelta = {}; 

public:
	static CTimer* Create(void);

};
END
