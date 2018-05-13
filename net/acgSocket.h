#pragma once

#include "../base/acgNoCopyable.h"

namespace acg
{
    namespace net
    {
        class CACGSocket :public base::CACGNoCopyable
        {
        public:
            explicit CACGSocket(SOCKET socket);
            ~CACGSocket();

        private:
            const SOCKET m_socket;
        };
    }
}