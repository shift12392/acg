// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "../targetver.h"

//���ͷ�ļ�Ҫ��acg_win.hͷ�ļ������
#include "../base/acg_switch.h"

//������ǰ�Windows 2008�������룬���̱���
#if _WIN32_WINNT < 0x0600 || WINVER < 0x0600
#error  ������������Ŀ��ƽ̨ΪWindows 2008 Server ���� Windows vista ������ƽ̨������ǰ����Ŀ��ƽ̨�İ汾�ƶ���̫�ͣ����� _WIN32_WINNT �� WINVER �ĺ궨��
#endif


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include "../base/acg_win.h"

#include "acg_sock.h"

