#include "acg_net_lib.h"
#include "acgSocket.h"


namespace acg
{
    namespace net
    {
        CACGSocket::CACGSocket(SOCKET socket)
            :m_socket(socket)
        {
        }


        CACGSocket::~CACGSocket()
        {
        }
    }
}
