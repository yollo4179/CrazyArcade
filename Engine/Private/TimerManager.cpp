#include "../Public/TimerManager.h"
#include"Timer.h"
CTimerManager::CTimerManager(void)
{
}

_float CTimerManager::GetTimeDelta(const _wstring& strTimerTag)
{
	const auto iter =  m_mapTimer.find(strTimerTag);
	if (m_mapTimer.end() == iter)
	{
		MSG_BOX("WrongTag_:Timer ");
		return 0.f; 
	}
	return (*iter).second->GetTimeDelta();
}

HRESULT CTimerManager::AddTimer(const _wstring& strTimerTag)
{
	CTimer* pFoundTimer = FindTimer(strTimerTag);

	if (nullptr == pFoundTimer)
	{
		shared_ptr<CTimer> pTimer (CTimer::Create());
		if (nullptr == pTimer) return E_FAIL;

		m_mapTimer.emplace(strTimerTag, pTimer);//복사 일어나니까 레퍼런스 1증가 ( 함수 끝나기전에)
		return  S_OK; 
	}
	MSG_BOX("Failed To Create and Add  Timer ");
	return E_FAIL;
}

void CTimerManager::UpdateTimeDelta(const _wstring& strTimerTag)
{

	CTimer* pFoundTimer = FindTimer(strTimerTag);
	if (nullptr == pFoundTimer) return; 
	pFoundTimer->UpdateTimer();
	return; 

}

CTimer* CTimerManager::FindTimer(const _wstring& strTimerTag)
{

	const auto iter = m_mapTimer.find(strTimerTag);
	
	if (m_mapTimer.end() == iter)
	{
		return nullptr;
	}

	return (*iter).second.get();
}

CTimerManager* CTimerManager::Create()
{
	CTimerManager* pInstance = new CTimerManager();

	if (nullptr== pInstance)
	{
		MSG_BOX("Failled To Create TimerManager");
		Safe_Delete(pInstance);
	}

	return pInstance;
}
