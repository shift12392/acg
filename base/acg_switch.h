

//acg���غ궨��ͷ�ļ�, ͨ���޸�����ļ��е�һЩ�꿪�ض�����Զ���acg������

#pragma once

//�ر�4995����
#pragma warning( disable : 4995 )
// �� Windows ͷ���ų�����ʹ�õ�����
#define VC_EXTRALEAN


//��windows.h���ų��������õ�APIs,����WinSock��API
/*
Winsock2.hͷ�ļ��ڲ���������Windows.hͷ�ļ��ĺ���Ԫ�أ����WinsockӦ�ó�����ͨ��û��"#include <Windows.h>"��
�����Ҫ"#include <Windows.h>"����Ӧ��֮ǰ���#define WIN32_LEAN_AND_MEAN�ꡣ
������ʷԭ��Windows.h��ͷĬ�ϰ���Windows�׽���1.1��Winsock.hͷ�ļ���
Winsock.hͷ�ļ��е���������Windows Sockets 2.0�����Winsock2.hͷ�ļ��е�������ͻ�� 
WIN32_LEAN_AND_MEAN��ɷ�ֹWindows.h��ͷ����Winsock.h��
https://docs.microsoft.com/zh-cn/windows/desktop/winsock/creating-a-basic-winsock-application
*/
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


//ֻ�����ڼ������İ�
#pragma setlocale("chinese-simplified")