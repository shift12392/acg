// acg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "base\acgException.h"
#include <assert.h>
#include <iostream>

int main()
{
	try
	{
		
		/*acg::base::CACGException e(GetLastError());*/
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
