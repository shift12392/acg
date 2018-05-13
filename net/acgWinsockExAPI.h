#pragma once


#include "acg_net_lib.h"
#include "../base/acg_DbgOut.h"
#include "../base/acgException.h"
#include "../base/acgNoCopyable.h"

using namespace acg::base;

namespace acg
{
    namespace net
    {
        class CACGWinsockExAPI:public CACGNoCopyable
        {
        private:
            LPFN_ACCEPTEX m_pfnAcceptEx;
            LPFN_CONNECTEX m_pfnConnectEx;
            LPFN_DISCONNECTEX m_pfnDisconnectEx;
            LPFN_GETACCEPTEXSOCKADDRS m_pfnGetAcceptExSockaddrs;
            LPFN_TRANSMITFILE m_pfnTransmitfile;
            LPFN_TRANSMITPACKETS m_pfnTransmitPackets;
            LPFN_WSARECVMSG m_pfnWSARecvMsg;
#if(_WIN32_WINNT >= 0x0600)
            LPFN_WSASENDMSG m_pfnWSASendMsg;
#endif

        public:
            CACGWinsockExAPI(SOCKET skTemp = INVALID_SOCKET)
            {
                //动态加载不成功，则强制
                if (!LoadAllFun(skTemp))
                {
                    ACG_ASSERT(FALSE);
                    exit(1);
                }
            }

            CACGWinsockExAPI(int af, int type, int protocol)
            {
                if (!LoadAllFun(af, type, protocol))
                {
                    ACG_ASSERT(FALSE);
                    exit(1);
                }
            }

            ~CACGWinsockExAPI()
            {
                //WSACleanup会帮我们清理
            }


        public:
            ACG_FORCEINLINE BOOL AcceptEx(
                SOCKET sListenSocket,
                SOCKET sAcceptSocket,
                PVOID lpOutputBuffer,
                DWORD dwReceiveDataLength,
                DWORD dwLocalAddressLength,
                DWORD dwRemoteAddressLength,
                LPDWORD lpdwBytesReceived,
                LPOVERLAPPED lpOverlapped
            )
            {
                ACG_ASSERT(NULL != m_pfnAcceptEx);
                return m_pfnAcceptEx(sListenSocket,
                    sAcceptSocket,
                    lpOutputBuffer,
                    dwReceiveDataLength,
                    dwLocalAddressLength,
                    dwRemoteAddressLength,
                    lpdwBytesReceived,
                    lpOverlapped);
            }

            ACG_FORCEINLINE BOOL ConnectEx(
                SOCKET s,
                const struct sockaddr FAR *name,
                int namelen,
                PVOID lpSendBuffer,
                DWORD dwSendDataLength,
                LPDWORD lpdwBytesSent,
                LPOVERLAPPED lpOverlapped
            )
            {
                ACG_ASSERT(NULL != m_pfnConnectEx);
                return m_pfnConnectEx(
                    s,
                    name,
                    namelen,
                    lpSendBuffer,
                    dwSendDataLength,
                    lpdwBytesSent,
                    lpOverlapped
                );
            }

            ACG_FORCEINLINE BOOL DisconnectEx(
                SOCKET s,
                LPOVERLAPPED lpOverlapped,
                DWORD  dwFlags,
                DWORD  dwReserved
            )
            {
                ACG_ASSERT(NULL != m_pfnDisconnectEx);
                return m_pfnDisconnectEx(s,
                    lpOverlapped,
                    dwFlags,
                    dwReserved);
            }

            ACG_FORCEINLINE VOID GetAcceptExSockaddrs(
                PVOID lpOutputBuffer,
                DWORD dwReceiveDataLength,
                DWORD dwLocalAddressLength,
                DWORD dwRemoteAddressLength,
                sockaddr **LocalSockaddr,
                LPINT LocalSockaddrLength,
                sockaddr **RemoteSockaddr,
                LPINT RemoteSockaddrLength
            )
            {
                ACG_ASSERT(NULL != m_pfnGetAcceptExSockaddrs);
                return m_pfnGetAcceptExSockaddrs(
                    lpOutputBuffer,
                    dwReceiveDataLength,
                    dwLocalAddressLength,
                    dwRemoteAddressLength,
                    LocalSockaddr,
                    LocalSockaddrLength,
                    RemoteSockaddr,
                    RemoteSockaddrLength
                );
            }

            ACG_FORCEINLINE BOOL TransmitFile(
                SOCKET hSocket,
                HANDLE hFile,
                DWORD nNumberOfBytesToWrite,
                DWORD nNumberOfBytesPerSend,
                LPOVERLAPPED lpOverlapped,
                LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers,
                DWORD dwReserved
            )
            {
                ACG_ASSERT(NULL != m_pfnTransmitfile);
                return m_pfnTransmitfile(
                    hSocket,
                    hFile,
                    nNumberOfBytesToWrite,
                    nNumberOfBytesPerSend,
                    lpOverlapped,
                    lpTransmitBuffers,
                    dwReserved
                );
            }

            ACG_FORCEINLINE BOOL TransmitPackets(
                SOCKET hSocket,
                LPTRANSMIT_PACKETS_ELEMENT lpPacketArray,
                DWORD nElementCount,
                DWORD nSendSize,
                LPOVERLAPPED lpOverlapped,
                DWORD dwFlags
            )
            {
                ACG_ASSERT(NULL != m_pfnTransmitPackets);
                return m_pfnTransmitPackets(
                    hSocket,
                    lpPacketArray,
                    nElementCount,
                    nSendSize,
                    lpOverlapped,
                    dwFlags
                );
            }

            ACG_FORCEINLINE INT WSARecvMsg(
                SOCKET s,
                LPWSAMSG lpMsg,
                LPDWORD lpdwNumberOfBytesRecvd,
                LPWSAOVERLAPPED lpOverlapped,
                LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
            )
            {
                ACG_ASSERT(NULL != m_pfnWSARecvMsg);
                return m_pfnWSARecvMsg(
                    s,
                    lpMsg,
                    lpdwNumberOfBytesRecvd,
                    lpOverlapped,
                    lpCompletionRoutine
                );
            }

#if(_WIN32_WINNT >= 0x0600)
            ACG_FORCEINLINE INT WSASendMsg(
                SOCKET s,
                LPWSAMSG lpMsg,
                DWORD dwFlags,
                LPDWORD lpNumberOfBytesSent,
                LPWSAOVERLAPPED lpOverlapped,
                LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
            )
            {
                ACG_ASSERT(NULL != m_pfnWSASendMsg);
                return m_pfnWSASendMsg(
                    s,
                    lpMsg,
                    dwFlags,
                    lpNumberOfBytesSent,
                    lpOverlapped,
                    lpCompletionRoutine
                );
            }
#endif


        private:
            BOOL LoadWSAFun(SOCKET &skTemp, GUID& funGuid, void *&pFun)
            {
                DWORD dwBytes = 0;
                pFun = NULL;
                try
                {
                    if (INVALID_SOCKET == skTemp)
                    {
                        ACG_ASSERT(FALSE);
                        throw base::CACGException(_T("传入了空的SOCKET句柄,无法完成扩展函数的载入!"));

                    }


                    if (SOCKET_ERROR == ::WSAIoctl(skTemp,
                        SIO_GET_EXTENSION_FUNCTION_POINTER,
                        &funGuid,
                        sizeof(GUID),
                        &pFun,
                        sizeof(pFun),
                        &dwBytes,
                        NULL,
                        NULL
                    ))
                    {
                        pFun = NULL;
                        throw base::CACGException(static_cast<DWORD>(WSAGetLastError()));
                    }


#ifdef _DEBUG
                    {
                        GUID guid = WSAID_ACCEPTEX;
                        if (IsEqualGUID(guid, funGuid))
                        {
                            ACG_DBGOUTW(L"AcceptEx 加载成功!\n");
                        }
                    }
                    {
                        GUID guid = WSAID_CONNECTEX;
                        if (IsEqualGUID(guid, funGuid))
                        {
                            ACG_DBGOUTW(L"ConnectEx 加载成功!\n");
                        }
                    }

                    {
                        GUID Guid = WSAID_DISCONNECTEX;
                        if (IsEqualGUID(Guid, funGuid))
                        {
                            ACG_DBGOUTW(L"DisconnectEx 加载成功!\n");
                        }
                    }

                    {
                        GUID Guid = WSAID_GETACCEPTEXSOCKADDRS;
                        if (IsEqualGUID(Guid, funGuid))
                        {
                            ACG_DBGOUTW(L"GetAcceptExSockaddrs 加载成功!\n");
                        }
                    }
                    {
                        GUID Guid = WSAID_TRANSMITFILE;
                        if (IsEqualGUID(Guid, funGuid))
                        {
                            ACG_DBGOUTW(L"TransmitFile 加载成功!\n");
                        }

                    }
                    {
                        GUID Guid = WSAID_TRANSMITPACKETS;
                        if (IsEqualGUID(Guid, funGuid))
                        {
                            ACG_DBGOUTW(L"TransmitPackets 加载成功!\n");
                        }

                    }
                    {
                        GUID Guid = WSAID_WSARECVMSG;
                        if (IsEqualGUID(Guid, funGuid))
                        {
                            ACG_DBGOUTW(L"WSARecvMsg 加载成功!\n");
                        }
                    }

#if(_WIN32_WINNT >= 0x0600)
                    {
                        GUID Guid = WSAID_WSASENDMSG;
                        if (IsEqualGUID(Guid, funGuid))
                        {
                            ACG_DBGOUTW(L"WSASendMsg 加载成功!\n");
                        }
                    }
#endif

#endif   //_DEBUG

                }
                catch (CACGException e)
                {
                    ACG_DBGOUTW(L"CGRSWinsockExAPI::LoadWSAFun Error:%s\n", e.GetReason());
                }

                return (NULL != pFun);

            }
            BOOL LoadAcceptExFun(SOCKET &skTemp)
            {
                GUID GuidAcceptEx = WSAID_ACCEPTEX;
                return LoadWSAFun(skTemp, GuidAcceptEx, (void*&)m_pfnAcceptEx);
            }
            BOOL LoadConnectExFun(SOCKET &skTemp)
            {
                GUID GuidAcceptEx = WSAID_CONNECTEX;
                return LoadWSAFun(skTemp, GuidAcceptEx, (void*&)m_pfnConnectEx);
            }

            BOOL LoadDisconnectExFun(SOCKET&skTemp)
            {
                GUID GuidDisconnectEx = WSAID_DISCONNECTEX;
                return LoadWSAFun(skTemp, GuidDisconnectEx, (void*&)m_pfnDisconnectEx);
            }

            BOOL LoadGetAcceptExSockaddrsFun(SOCKET &skTemp)
            {
                GUID GuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
                return LoadWSAFun(skTemp, GuidGetAcceptExSockaddrs, (void*&)m_pfnGetAcceptExSockaddrs);
            }

            BOOL LoadTransmitFileFun(SOCKET&skTemp)
            {
                GUID GuidTransmitFile = WSAID_TRANSMITFILE;
                return LoadWSAFun(skTemp, GuidTransmitFile, (void*&)m_pfnTransmitfile);
            }

            BOOL LoadTransmitPacketsFun(SOCKET&skTemp)
            {
                GUID GuidTransmitPackets = WSAID_TRANSMITPACKETS;
                return LoadWSAFun(skTemp, GuidTransmitPackets, (void*&)m_pfnTransmitPackets);
            }

            BOOL LoadWSARecvMsgFun(SOCKET&skTemp)
            {
                GUID GuidWSARecvMsg = WSAID_WSARECVMSG;
                return LoadWSAFun(skTemp, GuidWSARecvMsg, (void*&)m_pfnWSARecvMsg);
            }

#if(_WIN32_WINNT >= 0x0600)
            BOOL LoadWSASendMsgFun(SOCKET&skTemp)
            {
                GUID GuidWSASendMsg = WSAID_WSASENDMSG;
                return LoadWSAFun(skTemp, GuidWSASendMsg, (void*&)m_pfnWSASendMsg);
            }
#endif

         private:
             BOOL LoadAllFun(SOCKET skTemp)
             {//注意这个地方的调用顺序，是根据服务器的需要，并结合了表达式副作用
              //而特意安排的调用顺序
                 BOOL bCreateSocket = FALSE;
                 BOOL bRet = FALSE;
                 try
                 {
                     if (INVALID_SOCKET == skTemp)
                     {//如果传入空的SOCKET句柄,那么就默认以TCP协议来创建SOCKET句柄
                      //这样加载的扩展函数只用于TCP协议工作
                         skTemp = ::WSASocket(AF_INET,
                             SOCK_STREAM,
                             IPPROTO_TCP,
                             NULL,
                             0,
                             WSA_FLAG_OVERLAPPED);

                         bCreateSocket = (skTemp != INVALID_SOCKET);
                         if (!bCreateSocket)
                         {
                             throw CACGException((DWORD)WSAGetLastError());
                         }
                     }
                     bRet = (LoadAcceptExFun(skTemp) &&
                         LoadGetAcceptExSockaddrsFun(skTemp) &&
                         LoadTransmitFileFun(skTemp) &&
                         LoadTransmitPacketsFun(skTemp) &&
                         LoadDisconnectExFun(skTemp) &&
                         LoadConnectExFun(skTemp) &&
                         LoadWSARecvMsgFun(skTemp) &&
#if(_WIN32_WINNT >= 0x0600)
                         LoadWSASendMsgFun(skTemp));
#endif
                 }
                 catch (CACGException e)
                 {
                     ACG_DBGOUTW(L"CGRSWinsockExAPI::LoadAllFun Error:%s\n", e.GetReason());
                 }

                 if (bCreateSocket)
                 {
                     closesocket(skTemp);
                 }
                 return bRet;
             }

             BOOL LoadAllFun(int af, int type, int protocol)
             {
                 BOOL bRet = FALSE;
                 SOCKET skTemp = INVALID_SOCKET;
                 try
                 {
                     skTemp = ::WSASocket(af,
                         type,
                         protocol,
                         NULL,
                         0,
                         WSA_FLAG_OVERLAPPED);

                     if (INVALID_SOCKET == skTemp)
                     {
                         throw CACGException((DWORD)WSAGetLastError());
                     }

                     bRet = LoadAllFun(skTemp);
                 }
                 catch (CACGException e)
                 {
                     ACG_DBGOUTW(L"CGRSWinsockExAPI::LoadAllFun Error:%s\n", e.GetReason());
                 }
                 if (INVALID_SOCKET != skTemp)
                 {
                     closesocket(skTemp);
                 }
                 return bRet;
             }

        };

    }

}