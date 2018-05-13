// acg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../base/acgException.h"
#include "../base/acgTimestamp.h"
#include "../base/acgLoging.h"
#include "../net/acgSockStartup.h"


using namespace acg;
using namespace acg::base;
using namespace acg::net;

int main()
{
	try
	{
        CACGTimestamp time(CACGTimestamp::now());
        CACGString str= time.ToString();

        //Logger::SetLogLevel(Logger::em_WARN);

        LOG_DEBUG << L"aaaa";

        CACGSockStartup init;



	}catch(acg::base::CACGException e)
	{
		e.GetErrCode();
	}
}
