

#include "acg_base_lib.h"
#include "acgLogStream.h"


#include <type_traits>


namespace acg
{
	namespace detail
	{
		const WCHAR digits[] = L"9876543210123456789";
		const WCHAR* zero = digits + 9;
		//BOOST_STATIC_ASSERT(sizeof(digits) == 20);

		const WCHAR digitsHex[] = L"0123456789ABCDEF";
		//BOOST_STATIC_ASSERT(sizeof digitsHex == 17);

		// Efficient Integer to String Conversions, by Matthew Wilson.
		template<typename T>
		size_t convert(WCHAR buf[], T value)
		{
			T i = value;
            WCHAR* p = buf;

			do
			{
				int lsd = static_cast<int>(i % 10);
				i /= 10;
				*p++ = zero[lsd];
			} while (i != 0);

			if (value < 0)
			{
				*p++ = L'-';
			}
			*p = L'\0';
			std::reverse(buf, p);

			return p - buf;
		}

		size_t convertHex(WCHAR buf[], uintptr_t value)
		{
			uintptr_t i = value;
            WCHAR* p = buf;

			do
			{
				int lsd = static_cast<int>(i % 16);
				i /= 16;
				*p++ = digitsHex[lsd];
			} while (i != 0);

			*p = L'\0';
			std::reverse(buf, p);

			return p - buf;
		}




        //注：以下两种声明，只声明了模板的这两种类型
        template class FixedBuffer<kLargeBuffer>;
        template class FixedBuffer<kSmallBuffer>;

        //class FixedBuffer  这些在源文件中的实现只对应上面两种类型
        //当SIZE为其他值时，编译器会提示找不到这些实现。
        template <int SIZE>
        const TCHAR* FixedBuffer<SIZE>::debugString()
        {
            *m_pszCul = L'\0';
            return m_szData;
        }

        template<int SIZE>
        void FixedBuffer<SIZE>::cookieStart()
        {
        }

        template<int SIZE>
        void FixedBuffer<SIZE>::cookieEnd()
        {
        }




	}

	namespace base
	{
		
		//class LogStream
		void LogStream::staticCheck()
		{
			//BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10);
			//BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10);
			//BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10);
			//BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10);
		}

		template <typename T>
		void LogStream::formatInteger(T v)
		{
			if (buffer_.avail() >= kMaxNumericSize)
			{
				size_t len= detail::convert(buffer_.current(), v);
				buffer_.add(len);
			}
		}

		LogStream& LogStream::operator<<(short v)
		{
			*this << static_cast<int>(v);
			return *this;
		}

		LogStream& LogStream::operator<<(unsigned short v)
		{
			*this << static_cast<unsigned int>(v);
			return *this;
		}

		LogStream& LogStream::operator<<(int v)
		{
			formatInteger(v);
			return *this;
		}

		LogStream& LogStream::operator<<(unsigned int v)
		{
			formatInteger(v);
			return *this;
		}

		LogStream& LogStream::operator<<(long v)
		{
			formatInteger(v);
			return *this;
		}

		LogStream& LogStream::operator<<(unsigned long v)
		{
			formatInteger(v);
			return *this;
		}

		LogStream& LogStream::operator<<(long long v)
		{
			formatInteger(v);
			return *this;
		}

		LogStream& LogStream::operator<<(unsigned long long v)
		{
			formatInteger(v);
			return *this;
		}


		LogStream& LogStream::operator<<(const void* p)
		{
			uintptr_t v = reinterpret_cast<uintptr_t>(p);
			if (buffer_.avail() >= kMaxNumericSize)
			{
				WCHAR* buf = buffer_.current();
				buf[0] = L'0';
				buf[1] = L'x';
				size_t len = detail::convertHex(buf + 2, v);
				buffer_.add(len + 2);
			}
			return *this;
		}


		// FIXME: replace this with Grisu3 by Florian Loitsch.
		LogStream& LogStream::operator<<(double v)
		{
			if (buffer_.avail() >= kMaxNumericSize)
			{
				int len = swprintf_s(buffer_.current(), kMaxNumericSize, L"%.12g", v);
				if (-1 == len)
					exit(0);
				buffer_.add(len);
			}
			return *this;
		}


		template <typename T>
		Fmt::Fmt(const WCHAR *str, T v)
		{
			ACG_ASSERT(std::is_arithmetic<T>::value == true);
			length_ = swprintf_s(buf_, ACG_COUNTOF(buf_), str, v);
			ACG_ASSERT(static_cast<size_t>(length_) < ACG_COUNTOF(buf_));
			if (-1 == length_)
				exit(0);
		}

		template Fmt::Fmt(const WCHAR* fmt, char);

		template Fmt::Fmt(const WCHAR* fmt, short);
		template Fmt::Fmt(const WCHAR* fmt, unsigned short);
		template Fmt::Fmt(const WCHAR* fmt, int);
		template Fmt::Fmt(const WCHAR* fmt, unsigned int);
		template Fmt::Fmt(const WCHAR* fmt, long);
		template Fmt::Fmt(const WCHAR* fmt, unsigned long);
		template Fmt::Fmt(const WCHAR* fmt, long long);
		template Fmt::Fmt(const WCHAR* fmt, unsigned long long);

		template Fmt::Fmt(const WCHAR* fmt, float);
		template Fmt::Fmt(const WCHAR* fmt, double);


	}
}