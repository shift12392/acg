


//�Ӵ����������඼���ܸ���


#pragma once
#include "acg_dll.h"

namespace acg
{
	namespace base
	{
		class ACG_DLL CACGNoCopyable
		{
		protected:
			CACGNoCopyable() {}
			~CACGNoCopyable() {}
		private:
			CACGNoCopyable(const CACGNoCopyable &);
			CACGNoCopyable & operator=(const CACGNoCopyable &);
		};
	}
}
