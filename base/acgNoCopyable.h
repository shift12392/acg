


//从此类派生的类都不能复制


#pragma once

namespace acg
{
	namespace base
	{
		class CACGNoCopyable
		{
		protected:
			CACGNoCopyable() = default;
			~CACGNoCopyable() = default;
		private:
			CACGNoCopyable(const CACGNoCopyable &) = delete;
			CACGNoCopyable & operator=(const CACGNoCopyable &) = delete;
		};
	}
}
