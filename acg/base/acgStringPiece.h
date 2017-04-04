#pragma once



#include "acgTypes.h"
#include <string.h>

namespace acg
{
	namespace base
	{
		class CACGStringArg
		{
		public:
			CACGStringArg(const char* str)
				:m_str(str)
			{ }

			CACGStringArg(const string& str)
				:m_str(str.c_str())
			{ }
			const char *c_str() { return m_str; }
		private:
			const char* m_str=NULL;
		};


		class CACGStringPiece
		{
		private:
			const char *m_pStr = NULL;
			int         m_nLength = 0;


		public:
			CACGStringPiece()
				: m_pStr(NULL), m_nLength(0) { }

			CACGStringPiece(const char* str)
				: m_pStr(str), m_nLength(static_cast<int>(strlen(m_pStr))) { }

			CACGStringPiece(const unsigned char *str)
				:m_pStr(reinterpret_cast<const char *>(str))
				,m_nLength(static_cast<int>(strlen(m_pStr))){ }

			CACGStringPiece(const string& str)
				: m_pStr(str.data()), m_nLength(static_cast<int>(str.size())) { }

			CACGStringPiece(const char* offset, int len)
				: m_pStr(offset), m_nLength(len) { }



			const char * data() const { return m_pStr; }
			int size() const { return m_nLength; }
			bool empty() { return m_nLength == 0; }
			const char * begin() { return m_pStr; }
			const char * end() { return m_pStr + m_nLength; }

			void clear() { m_pStr = NULL; m_nLength = 0; }
			void set(const char* buffer, int len) { m_pStr = buffer; m_nLength = len; }
			void set(const char* str) {
				m_pStr = str;
				m_nLength = static_cast<int>(strlen(str));
			}
			void set(const void* buffer, int len) {
				m_pStr = reinterpret_cast<const char*>(buffer);
				m_nLength = len;
			}

			char operator[](int i) const { return m_pStr[i]; }

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
				(memcmp(m_pStr, x.m_pStr, m_nLength) == 0));
			}
			bool operator!=(const CACGStringPiece& x) const
			{
				return !(*this == x);
			}

#define STRINGPIECE_BINARY_PREDICATE(cmp,auxcmp)                                                   \
		  bool operator cmp (const CACGStringPiece& x) const {                                     \
			int r = memcmp(m_pStr, x.m_pStr, m_nLength < x.m_nLength ? m_nLength : x.m_nLength);   \
			return ((r auxcmp 0) || ((r == 0) && (m_nLength cmp x.m_nLength)));                    \
		  }
		STRINGPIECE_BINARY_PREDICATE(<, <);
		STRINGPIECE_BINARY_PREDICATE(<= , <);
		STRINGPIECE_BINARY_PREDICATE(>= , >);
		STRINGPIECE_BINARY_PREDICATE(>, >);
#undef STRINGPIECE_BINARY_PREDICATE


		int compare(const CACGStringPiece& x) const
		{
			int r = memcmp(m_pStr, x.m_pStr, m_nLength < x.m_nLength ? m_nLength : x.m_nLength);
			if (r == 0) {
				if (m_nLength < x.m_nLength) r = -1;
				else if (m_nLength > x.m_nLength) r = +1;
			}
			return r;
		}

		string as_string() const
		{
			return string(data(), size());
		}

		void CopyToString(string* target) const
		{
			target->assign(m_pStr, m_nLength);
		}

		// Does "this" start with "x"
		bool starts_with(const CACGStringPiece& x) const
		{
			return ((m_nLength >= x.m_nLength) && (memcmp(m_pStr, x.m_pStr, x.m_nLength) == 0));
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
template<> struct __type_traits<muduo::StringPiece> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};
#endif

// allow StringPiece to be logged
std::ostream& operator<<(std::ostream& o, const acg::base::CACGStringPiece& piece);



