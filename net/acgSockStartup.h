#pragma once


#include "../base/acgNoCopyable.h"
#include "../base/acg_DbgOut.h"
#include "../base/acgLoging.h"
#include "acg_sock.h"


namespace acg
{
    namespace net
    {
        class CACGSockStartup :public base::CACGNoCopyable
        {
        private:
            WSADATA m_wsaData;
            BOOL    m_bInited = FALSE;
        public:
            CACGSockStartup(BYTE bvHight = 0x2, BYTE bvLow = 0x2)
            {
                memset(&m_wsaData, 0, sizeof(WSADATA));
                if (!InitSock(bvHight, bvLow))
                {
                    exit(1);
                }
            }
            ~CACGSockStartup()
            {
                UninitSock();
            }

        public:
            BOOL InitSock(BYTE bvHigh, BYTE bvLow)
            {
                if (m_bInited)
                {
                    LOG_DEBUG << "重复初始化";
                    return FALSE;
                }

				//The WSAStartup function is called to initiate use of WS2_32.dll.
                WORD wVer = MAKEWORD(bvHigh, bvLow);
                int err = ::WSAStartup(wVer, &m_wsaData);
                if (0 != err)
                {
                    //ACG_DBGOUTW(_T("无法初始化Socket2系统环境，错误码为：%d！\n"), WSAGetLastError());

					//直接杀死进程
                    LOG_FATAL << L"无法初始化Socket2系统环境，错误码为：%d！" << WSAGetLastError();

                    return FALSE;
                }
                if (LOBYTE(m_wsaData.wVersion) != bvHigh ||
                    HIBYTE(m_wsaData.wVersion) != bvLow)
                {
                    //ACG_DBGOUTW(_T("无法初始化%d.%d版本的Socket环境！\n"), bvHigh, bvLow);
                    LOG_FATAL << L"无法初始化%d.%d版本的Socket环境！高版本号：" << bvHigh <<L" 低版本号：" << bvLow;
                    WSACleanup();
                    return FALSE;
                }

                //ACG_DBGOUTW(_T("Winsock库初始化成功!\n\t系统中支持最高的Winsock版本为%d.%d\n\t当前应用加载的版本为%d.%d\n")
                //    , LOBYTE(m_wsaData.wHighVersion), HIBYTE(m_wsaData.wHighVersion)
                 //   , LOBYTE(m_wsaData.wVersion), HIBYTE(m_wsaData.wVersion));

                LOG_DEBUG << L"Winsock库初始化成功!\n\t系统中支持最高的Winsock版本为"
                    << LOBYTE(m_wsaData.wHighVersion)
                    << L"." << HIBYTE(m_wsaData.wHighVersion) << L"\n\t"
                    << L"当前应用加载的版本为" << LOBYTE(m_wsaData.wVersion) << L"." << HIBYTE(m_wsaData.wVersion);


                m_bInited = TRUE;

                return TRUE;
            }


            VOID UninitSock()
            {
                WSACleanup();
                ACG_DBGOUTW(_T("Winsock库已被卸载,环境已经被释放!\n\t系统中支持最高的Winsock版本为%d.%d\n\t应用加载的版本为%d.%d\n")
                    , LOBYTE(m_wsaData.wHighVersion), HIBYTE(m_wsaData.wHighVersion)
                    , LOBYTE(m_wsaData.wVersion), HIBYTE(m_wsaData.wVersion));

                m_bInited = FALSE;
            }

            WORD GetRunVersion()
            {
                return m_wsaData.wVersion;
            }

            WORD GetSysHighVersion()
            {
                return m_wsaData.wHighVersion;
            }

            BYTE GetRunMajorVersion()
            {
                return LOBYTE(m_wsaData.wVersion);
            }

            BYTE GetRunMinorVersion()
            {
                return HIBYTE(m_wsaData.wVersion);
            }

            BYTE GetSysHighMajorVersion()
            {
                return LOBYTE(m_wsaData.wHighVersion);
            }

            BYTE GetSysHighMinorVersion()
            {
                return HIBYTE(m_wsaData.wHighVersion);
            }


        };
    }
}