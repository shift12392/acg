#pragma once

#include "acgStringPiece.h"
#include "acg_type.h"
#include "acg_DbgOut.h"
#include "acgNoCopyable.h"


namespace acg
{

    namespace detail
    {
        const int kSmallBuffer = 4096;
        const int kLargeBuffer = 4096 * 1000;


        template<int SIZE>
        class FixedBuffer :public base::CACGNoCopyable
        {
        public:
            FixedBuffer()
                : m_pszCul(m_szData)
            {
                setCookie(cookieStart);
            }

            ~FixedBuffer()
            {
                setCookie(cookieEnd);
            }

            void append(const WCHAR *buf, size_t len)
            {
                if (base::implicit_cast<size_t>(avail()) > len)
                {
                    memcpy(m_pszCul, buf, sizeof(WCHAR)*len);
                    m_pszCul += len;
                }
            }

            const WCHAR* data() const { return m_szData; }
            int length() const { return static_cast<int>(m_pszCul - m_szData); }

            // write to data_ directly
            WCHAR* current() const { return m_pszCul; }
            int avail() const { return static_cast<int>(end() - m_pszCul); }
            void add(size_t len) { m_pszCul += len; }

            void reset() { m_pszCul = m_szData; }
            void bzero() { memset(m_szData, 0, sizeof(m_szData)); }

            // for used by GDB
            const WCHAR* debugString();
            void setCookie(void(*cookie)()) { cookie_ = cookie; }

            // for used by unit test
            base::CACGString toString() const { return base::CACGString(m_szData, length()); }
            base::CACGStringPiece toStringPiece() const { return base::CACGStringPiece(m_szData, length()); }


        private:
            const WCHAR* end() const { return m_szData + ACG_COUNTOF(m_szData); }
            // Must be outline function for cookies.
            static void cookieStart();
            static void cookieEnd();

            void(*cookie_)();
            WCHAR m_szData[SIZE];
            WCHAR* m_pszCul;
        };
    }

	namespace base
	{

		class LogStream :public CACGNoCopyable
		{
			typedef LogStream self;
		public:
			typedef detail::FixedBuffer<detail::kSmallBuffer>   Buffer;

			self& operator<<(bool v)
			{
				buffer_.append(v ? L"1" : L"0", 1);
				return *this;
			}

			self& operator<<(short);
			self& operator<<(unsigned short);
			self& operator<<(int);
			self& operator<<(unsigned int);
			self& operator<<(long);
			self& operator<<(unsigned long);
			self& operator<<(long long);
			self& operator<<(unsigned long long);

			self& operator<<(const void*);               //¶þ½øÖÆ

			self& operator<<(float v)
			{
				*this << static_cast<double>(v);
				return *this;
			}
			self& operator<<(double);
			// self& operator<<(long double);

			self& operator<<(WCHAR v)
			{
				buffer_.append(&v, 1);
				return *this;
			}
			// self& operator<<(signed char);
			// self& operator<<(unsigned char);

			self& operator<<(const WCHAR *str)
			{
				if (str)
				{
					size_t  strLen;
                    ACG_CHECK_STR(StringCchLengthW(str, detail::kSmallBuffer, &strLen));
					buffer_.append(str, strLen);
				}
				else
				{
					buffer_.append(L"(null)", 6);
				}

				return *this;
			}

			//self& operator<<(const unsigned char* str)
			//{
			//	return operator<<(reinterpret_cast<const TCHAR*>(str));
			//}

			self& operator<<(const CACGString& v)
			{
				buffer_.append(v.c_str(), v.size());
				return *this;
			}

			self& operator<<(const CACGStringPiece& v)
			{
				buffer_.append(v.data(), v.size());
				return *this;
			}

			self& operator<<(const Buffer& v)
			{
				*this << v.toStringPiece();
				return *this;
			}

			void append(const WCHAR* data, int len) { buffer_.append(data, len); }
			const Buffer& buffer() const { return buffer_; }
			void resetBuffer() { buffer_.reset(); }

		private:
			void staticCheck();

			template <typename T>
			void formatInteger(T v);


			Buffer buffer_;
			static const int kMaxNumericSize = 64;
		};

		class Fmt
		{
		public:
			template <typename T>
			Fmt(const WCHAR *str, T v);

			const WCHAR* data() const { return buf_; }
			int length() const { return length_; }

		private:
            WCHAR buf_[64];
			int  length_;
		};

		inline LogStream& operator<<(LogStream& stream, const Fmt& fmt)
		{
			stream.append(fmt.data(), fmt.length());
			return stream;
		}

	}
}
