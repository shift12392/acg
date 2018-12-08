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
                    LOG_DEBUG << "�ظ���ʼ��";
                    return FALSE;
                }

				//The WSAStartup function is called to initiate use of WS2_32.dll.
                WORD wVer = MAKEWORD(bvHigh, bvLow);
                int err = ::WSAStartup(wVer, &m_wsaData);
                if (0 != err)
                {
                    //ACG_DBGOUTW(_T("�޷���ʼ��Socket2ϵͳ������������Ϊ��%d��\n"), WSAGetLastError());

					//ֱ��ɱ������
                    LOG_FATAL << L"�޷���ʼ��Socket2ϵͳ������������Ϊ��%d��" << WSAGetLastError();

                    return FALSE;
                }
                if (LOBYTE(m_wsaData.wVersion) != bvHigh ||
                    HIBYTE(m_wsaData.wVersion) != bvLow)
                {
                    //ACG_DBGOUTW(_T("�޷���ʼ��%d.%d�汾��Socket������\n"), bvHigh, bvLow);
                    LOG_FATAL << L"�޷���ʼ��%d.%d�汾��Socket�������߰汾�ţ�" << bvHigh <<L" �Ͱ汾�ţ�" << bvLow;
                    WSACleanup();
                    return FALSE;
                }

                //ACG_DBGOUTW(_T("Winsock���ʼ���ɹ�!\n\tϵͳ��֧����ߵ�Winsock�汾Ϊ%d.%d\n\t��ǰӦ�ü��صİ汾Ϊ%d.%d\n")
                //    , LOBYTE(m_wsaData.wHighVersion), HIBYTE(m_wsaData.wHighVersion)
                 //   , LOBYTE(m_wsaData.wVersion), HIBYTE(m_wsaData.wVersion));

                LOG_DEBUG << L"Winsock���ʼ���ɹ�!\n\tϵͳ��֧����ߵ�Winsock�汾Ϊ"
                    << LOBYTE(m_wsaData.wHighVersion)
                    << L"." << HIBYTE(m_wsaData.wHighVersion) << L"\n\t"
                    << L"��ǰӦ�ü��صİ汾Ϊ" << LOBYTE(m_wsaData.wVersion) << L"." << HIBYTE(m_wsaData.wVersion);


                m_bInited = TRUE;

                return TRUE;
            }


            VOID UninitSock()
            {
                WSACleanup();
                ACG_DBGOUTW(_T("Winsock���ѱ�ж��,�����Ѿ����ͷ�!\n\tϵͳ��֧����ߵ�Winsock�汾Ϊ%d.%d\n\tӦ�ü��صİ汾Ϊ%d.%d\n")
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