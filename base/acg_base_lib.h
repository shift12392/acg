// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "../targetver.h"

#include "acg_switch.h"


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

//������ǰ�Windows 2008�������룬���̱���
#if _WIN32_WINNT < 0x0600 || WINVER < 0x0600
#error  ������������Ŀ��ƽ̨ΪWindows 2008 Server ���� Windows vista ������ƽ̨������ǰ����Ŀ��ƽ̨�İ汾�ƶ���̫�ͣ����� _WIN32_WINNT �� WINVER �ĺ궨��
#endif


//UNICODE Windows֧��
#include <tchar.h>
#include <windows.h>
#include <locale.h>
//ʹ��ϵͳ��ȫ�ַ�������֧��
#include <strsafe.h>
//ʹ��ATL���ַ���ת��֧��
#include <atlconv.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "acg_def.h"
