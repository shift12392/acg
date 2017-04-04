#pragma once

#include "acgNoCopyable.h"

namespace acg
{
	namespace base
	{
		template<typename T>
		class CACGAtomicT : public CACGNoCopyable
		{
		public:

			T Get()
			{
				return InterlockedCompareExchange(&m_nNum, 0, 0);
			}

			T GetAndAdd(T x)
			{
				return InterlockedExchangeAdd(&m_nNum, x);           //注：这些原子函数要求第一个传入地址必须已经对齐
			}

			T AddAndGet(T x)
			{
				return GetAndAdd(x) + x;
			}

			T IncrementAndGet()
			{
				return AddAndGet(1);
			}

			T DecrementAndGet()
			{
				return AddAndGet(-1);
			}

			void Add(T x)
			{
				return AddAndGet(x);
			}

			void Increment()
			{
				IncrementAndGet();
			}

			void Decrement()
			{
				DecrementAndGet();
			}

			T GetAndSet(T newValue)
			{
				return InterlockedExchange(&m_nNum, newValue);
			}

		private:
			T m_nNum = 0;               //vc++默认类内是8字节对齐的，对齐设置在c/c++ -- 代码生成 -- 结构数据对齐中设定
		};

		typedef CACGAtomicT<unsigned int> CACGAtomic32;
		typedef CACGAtomicT<unsigned long long> CACGAtomic64;
	}
}