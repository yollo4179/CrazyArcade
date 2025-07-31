#include "Timer.h"

CTimer::CTimer()
    :m_fTimeDelta(0.f)
{
    ZeroMemory(&m_OneSecAgoTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}
HRESULT CTimer::ReadyTimer(void)
{
    QueryPerformanceCounter(&m_FrameTime);			// 1077
    QueryPerformanceCounter(&m_LastTime);			// 1085
    QueryPerformanceCounter(&m_OneSecAgoTime);			// 1090
  
    QueryPerformanceFrequency(&m_CpuTick);
    return S_OK;
}


void CTimer::UpdateTimer(void)
{
    //// 1초에 몇 번 카운터가 증가하는지?
    //QueryPerformanceFrequency(&frequency);
    //// 시작 시간
    //QueryPerformanceCounter(&start);
    //// 예시 작업
    //Sleep(100); // 100ms 대기
    //// 끝 시간
    //QueryPerformanceCounter(&end);
    //// 경과 시간 계산 (초 단위)
    //double elapsedSeconds = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    /*Todo Delta Time 구하기 */
    /*
    *프레임 간격사이의 실제 시간 구하기  
    * 
    * 1. 프레임사이의 틱수 =현재 프레임의 시간 ( 틱수) - 이전 프레임의 시간  (틱수)
    *  2. Delta Time =프레임 사이의 지난 시간 = 프레임사이의 틱수 /현재 까지의 1 초당 틱수
    */
    
    
    QueryPerformanceCounter(&m_FrameTime);
    if (m_FrameTime.QuadPart - m_OneSecAgoTime.QuadPart >= m_CpuTick.QuadPart) //1초마다 갱신
    {
        QueryPerformanceFrequency(&m_CpuTick);
        m_OneSecAgoTime = m_FrameTime;

    }


    /*프레임당 지난 시간-Always*/
    m_fTimeDelta = (m_FrameTime.QuadPart - m_LastTime.QuadPart)/(_float) m_CpuTick.QuadPart;
    m_LastTime = m_FrameTime;
        
}

CTimer* CTimer::Create(void)
{
    CTimer* m_pInstance = new CTimer();
    if (FAILED(m_pInstance->ReadyTimer()))
    {
        MSG_BOX("Failed To Create Timer:");
        Safe_Delete(m_pInstance); 
    }
    return m_pInstance;
}
