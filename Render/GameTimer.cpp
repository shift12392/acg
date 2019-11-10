#include "stdafx.h"
#include "GameTimer.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
// Windows ͷ�ļ�
#include <windows.h>

#include <assert.h>
#include <stdlib.h>


GameTimer::GameTimer()
{
	LARGE_INTEGER frequency;
	BOOL bRet = QueryPerformanceFrequency(&frequency);
	if (!bRet)
	{
		assert(false);
		exit(1);
	}

	m_SecondsPerCount = 1.0 / (double)frequency.QuadPart;
}


GameTimer::~GameTimer()
{
}

void GameTimer::Reset()
{
	BOOL bRet = QueryPerformanceCounter(&m_CurrentCount);
	if (!bRet)
	{
		assert(false);
		m_CurrentCount.QuadPart = 0;
	}
	m_BaseCount = m_CurrentCount;
	m_PrevCount = m_CurrentCount;
	//m_PausedCount.QuadPart = 0;
	m_StopCount.QuadPart = 0;
	m_DeltaTime = 0.0;
	
	m_bStoped = false;
}

void GameTimer::Start()
{
	LARGE_INTEGER startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_bStoped)
	{
		m_PausedCount.QuadPart += (startTime.QuadPart - m_StopCount.QuadPart);

		m_PrevCount.QuadPart = startTime.QuadPart;
		m_StopCount.QuadPart = 0;
		m_bStoped = false;
	}
}

void GameTimer::Stop()
{
	if (!m_bStoped)
	{
		BOOL bRet = QueryPerformanceCounter(&m_StopCount);
		if (!bRet)
		{
			assert(false);
			m_StopCount.QuadPart = 0;
		}

		m_bStoped = true;
	}

	
}

double GameTimer::Tick()
{
	if (m_bStoped)
	{
		m_DeltaTime = 0.0;
		return m_DeltaTime;
	}

	BOOL bRet = QueryPerformanceCounter(&m_CurrentCount);
	if (!bRet)
	{
		assert(false);
		m_CurrentCount = m_PrevCount;
		m_DeltaTime = 0.0;
		return m_DeltaTime;
	}
	else
	{
		m_DeltaTime = (m_CurrentCount.QuadPart - m_PrevCount.QuadPart) * m_SecondsPerCount;
		m_PrevCount = m_CurrentCount;
	}

	//�ĵ���˵��m_PrevCount��m_CurrentCount�ǲ�ͬ�Ĵ������ϵõ��Ľ�������ܻ���ָ�ֵ���������
	if (m_DeltaTime < 0.0)
	{
		assert(false);
		m_DeltaTime = 0.0;
	}

	return m_DeltaTime;
}
