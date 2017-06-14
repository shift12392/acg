#pragma once



#include "acg_type.h"
#include <string.h>


//注意，char和wchar_t的大小问题，使用memcpy,memset,memcmp或sizeof时特别注意。
//注意：要使用中文字符，一定要定义UNICODE。


namespace acg
{
	namespace base
	{
		class CACGStringArg
		{
		public:
			CACGStringArg(const WCHAR* str)
				:m_str(str)
			{ }

			CACGStringArg(const CACGString& str)
				:m_str(str.c_str())
			{ }
			const WCHAR *c_str() { return m_str; }
		private:
			const WCHAR* m_str=NULL;
		};


		class CACGStringPiece
		{
		private:
			const WCHAR *m_pStr = NULL;
			int         m_nLength = 0;


		public:
			CACGStringPiece()
				: m_pStr(NULL), m_nLength(0) { }

            CACGStringPiece(const WCHAR* str)
                : m_pStr(str), m_nLength(static_cast<int>(wcslen(m_pStr))){ }

			CACGStringPiece(const CACGString& str)
				: m_pStr(str.data()), m_nLength(static_cast<int>(str.size())) { }

			CACGStringPiece(const WCHAR* offset, int len)
				: m_pStr(offset), m_nLength(len) { }



			const WCHAR * data() const { return m_pStr; }
			int           size() const { return m_nLength; }
			bool          empty() const { return m_nLength == 0; }
			const WCHAR * begin() const { return m_pStr; }
			const WCHAR * end() const { return m_pStr + m_nLength; }

			void clear() { m_pStr = NULL; m_nLength = 0; }
			void set(const WCHAR* buffer, int len) { m_pStr = buffer; m_nLength = len; }
			void set(const WCHAR* str) {
				m_pStr = str;
				m_nLength = static_cast<int>(wcslen(str));
			}
			void set(const void* buffer, int len) {
				m_pStr = reinterpret_cast<const WCHAR*>(buffer);
				m_nLength = len;
			}

            //如果没有定义UNICODE，m_pStr保存的有中文，则这样返回不了中文字符。
            //比如：m_pStr中保存的是"abc中国"，则m_pStr[3]!='中'。要特别注意这一点。
            WCHAR operator[](int i) const { return m_pStr[i]; }

			//去掉前面字符串
			void remove_prefix(int n) {
				m_pStr += n;
				m_nLength -= n;
			}

			//去掉后面字符串
			void remove_suffix(int n) {
				m_nLength -= n;
			}

			bool operator==(const CACGStringPiece &x) const
			{
				return ((m_nLength == x.m_nLength) &&
				(memcmp(m_pStr, x.m_pStr, sizeof(WCHAR)*m_nLength) == 0));
			}
			bool operator!=(const CACGStringPiece& x) const
			{
				return !(*this == x);
			}

#define STRINGPIECE_BINARY_PREDICATE(cmp,auxcmp)                                                   \
		  bool operator cmp (const CACGStringPiece& x) const {                                     \
			int r = memcmp(m_pStr, x.m_pStr, m_nLength < x.m_nLength ? sizeof(WCHAR)*m_nLength : sizeof(WCHAR)*(x.m_nLength));   \
			return ((r auxcmp 0) || ((r == 0) && (m_nLength cmp x.m_nLength)));                    \
		  }
		STRINGPIECE_BINARY_PREDICATE(<, <);
		STRINGPIECE_BINARY_PREDICATE(<= , <);
		STRINGPIECE_BINARY_PREDICATE(>= , >);
		STRINGPIECE_BINARY_PREDICATE(>, >);
#undef STRINGPIECE_BINARY_PREDICATE


		int compare(const CACGStringPiece& x) const
		{
			int r = memcmp(m_pStr, x.m_pStr, m_nLength < x.m_nLength ? sizeof(WCHAR)*m_nLength : sizeof(WCHAR)*(x.m_nLength));
			if (r == 0) {
				if (m_nLength < x.m_nLength) r = -1;
				else if (m_nLength > x.m_nLength) r = +1;
			}
			return r;
		}

		CACGString as_string() const
		{
			return CACGString(data(), size());
		}

		void CopyToString(CACGString* target) const
		{
			target->assign(m_pStr, m_nLength);
		}

		// Does "this" start with "x"
		bool starts_with(const CACGStringPiece& x) const
		{
			return ((m_nLength >= x.m_nLength) && (memcmp(m_pStr, x.m_pStr, sizeof(WCHAR)*(x.m_nLength)) == 0));
		}
		};
	}
}

// ------------------------------------------------------------------
// Functions used to create STL containers that use StringPiece
//  Remember that a StringPiece's lifetime had better be less than
//  that of the underlying string or char*.  If it is not, then you
//  cannot safely store a StringPiece into an STL container
// ------------------------------------------------------------------

#ifdef HAVE_TYPE_TRAITS
// This makes vector<StringPiece> really fast for some STL implementations
template<> struct __type_traits<acg::base::StringPiece> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};
#endif

// allow StringPiece to be logged
std::ostream& operator<<(std::ostream& o, const acg::base::CACGStringPiece& piece);



