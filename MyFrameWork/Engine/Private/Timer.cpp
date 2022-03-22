#include "Timer.h"

CTimer::CTimer()
	: m_dTimeDelta(0.f)

{	
	QueryPerformanceCounter(&m_CurrentTime);		// 1000
	QueryPerformanceCounter(&m_OldTime);			// 1020	
	QueryPerformanceCounter(&m_OriginTime);			// 1030														
	QueryPerformanceFrequency(&m_CpuTick);			// 1600000
}

_double CTimer::Get_TimeDelta(void)
{
	QueryPerformanceCounter(&m_CurrentTime);	// 2000	//	3000 // 4000 // 5000

	if (m_CurrentTime.QuadPart - m_OriginTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_OriginTime = m_CurrentTime;
	}

	m_dTimeDelta = _double(m_CurrentTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;

	m_OldTime = m_CurrentTime;

	return m_dTimeDelta;
}

CTimer * CTimer::Create()
{
	CTimer*	pInstance = new CTimer();
	
	return pInstance;
}

void CTimer::Free()
{
}

