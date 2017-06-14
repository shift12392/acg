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
				return InterlockedExchangeAdd(&m_nNum, x);           //ע����Щԭ�Ӻ���Ҫ���һ�������ַ�����Ѿ�����
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
			T m_nNum = 0;               //vc++Ĭ��������8�ֽڶ���ģ�����������c/c++ -- �������� -- �ṹ���ݶ������趨
		};

		typedef CACGAtomicT<unsigned int> CACGAtomic32;
		typedef CACGAtomicT<unsigned long long> CACGAtomic64;
	}
}