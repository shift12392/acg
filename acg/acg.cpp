// acg.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "base\acgException.h"
#include <assert.h>
#include <iostream>
#include "base\acgCurThread.h"
#include "base\acgAtomic.h"
#include "base\acgThread.h"
#include "base\acgTimestamp.h"
#include <time.h>
#include <limits>
#include "base\acgLogStream.h"


using namespace std;
using namespace acg;
using namespace acg::base;

void ThreadFunc()
{
	cout << CurThread::GetName() << " runing " << CurThread::GetTidString() << endl;
	Sleep(5000);
	cout << CurThread::GetName() << " finish " << endl;
}

// ��ȡUnixʱ�������λ΢��(us)���̰߳�ȫ
LONGLONG GetSysTimeMicros()
{
	// ��1601��1��1��0:0:0:000��1970��1��1��0:0:0:000��ʱ��(��λ100ns)
#define EPOCHFILETIME   (116444736000000000UL)
	FILETIME ft;
	LARGE_INTEGER li;
	LONGLONG tt = 0;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	// ��1970��1��1��0:0:0:000�����ڵ�΢����(UTCʱ��)
	tt = (li.QuadPart - EPOCHFILETIME) / 10;
#undef EPOCHFILETIME
	return tt;
}

int main()
{
	try
	{

		

		Fmt fmt("%lld", 400000);

		LogStream log;
		log << -10;
		log << 1.233333333;
		log << -10;
		log << "aaaa";
		log << fmt;

		cout << std::numeric_limits<long>::digits10 << endl;

		cout<< CACGTimestamp::now().ToString()<<endl;

		FILETIME ft = {};
		SYSTEMTIME sysTm = {};
		LARGE_INTEGER li = {};
		LONGLONG tt = 0;
		GetSystemTimeAsFileTime(&ft);
		FileTimeToSystemTime(&ft,&sysTm);



		LARGE_INTEGER m_nFreq;
		LARGE_INTEGER m_nBeginTime;
		LARGE_INTEGER nEndTime;
		QueryPerformanceFrequency(&m_nFreq); // ��ȡʱ������
		QueryPerformanceCounter(&m_nBeginTime); // ��ȡʱ�Ӽ���
		Sleep(100);
		QueryPerformanceCounter(&nEndTime);
		cout << (nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << endl;

		cout << CurThread::GetName () << " runing " << CurThread::GetTidString() << endl;

		CACGThread thread(ThreadFunc);
		thread.Start();

		acg::base::CACGAtomic32  num32;
		cout << num32.IncrementAndGet() << endl;
		cout << num32.IncrementAndGet() << endl;
		cout << num32.IncrementAndGet() << endl;
		cout << num32.DecrementAndGet() << endl;
		cout << num32.DecrementAndGet() << endl;
		cout << num32.Get() << endl;
		cout << num32.AddAndGet(2) << endl;
		cout << num32.GetAndAdd(2)<< endl;
		



		acg::base::CurThread::GetTid();

		acg::base::CACGException e(GetLastError());
		//std::cout << e.GetErrCode() << std::endl;

		int b = 0;
		std::cin >> b;
		int a = 10 / b;
		
	}catch(acg::base::CACGException e)
	{
		e.GetErrCode();
		assert(false);
	}
}
