


//从此类派生的类都不能复制


#pragma once

namespace acg
{
	namespace base
	{
		class CACGNoCopyable
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
