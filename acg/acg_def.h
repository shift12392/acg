
//һЩ���ú궨��
#pragma once
#define ACG_DEF_H


//�������������Ϣʱ�����������͵���Ϣ����ַ����͵�
#define ACG_STRING2(x) #x
#define ACG_STRING(x) ACG_STRING2(x)

#define ACG_WSTRING2(x) L #x
#define ACG_WSTRING(x) ACG_WSTRING2(x)

//__FILE__Ԥ�����Ŀ��ַ��汾
#define ACG_WIDEN2(x) L ## x
#define ACG_WIDEN(x) ACG_WIDEN2(x)
#define __WFILE__ ACG_WIDEN(__FILE__)

//TLS �洢֧��
#define ACG_TLS				__declspec( thread )

//��������
#ifndef ACG_INLINE
#define ACG_INLINE			__inline
#endif

#ifndef ACG_FORCEINLINE
#define ACG_FORCEINLINE		__forceinline
#endif

#ifndef ACG_NOVTABLE
#define ACG_NOVTABLE		__declspec(novtable)
#endif

#ifndef ACG_CDECL
#define ACG_CDECL			__cdecl
#endif

#ifndef ACG_STDCL 
#define ACG_STDCL			__stdcall
#endif

#ifndef ACG_FASTCL
#define ACG_FASTCL			__fastcall
#endif

#ifndef ACG_PASCAL
#define ACG_PASCAL			__pascal
#endif

//��鰲ȫ�ַ�����������ֵ�����ʧ�ܣ���ɱ������
#define ACG_CHECK_STR(ret) if(S_OK!=(ret)) exit(0);

//��ȫdelete
#ifndef ACG_SAFEDEL
#define ACG_SAFEDEL(p) if(NULL!=(p)) delete (p);
#endif

//ȡ��̬����Ԫ�صĸ���
#ifndef ACG_COUNTOF
#define ACG_COUNTOF(a)		sizeof(a)/sizeof(a[0])
#endif

//�������������ȡ������3/2 = 2
#ifndef ACG_UPDIV
#define ACG_UPDIV(a,b) ((a) + (b) - 1)/(b)
#endif

//��a���뵽b��������
#ifndef ACG_UPROUND
#define ACG_UPROUND(x,y) (((x)+((y)-1))&~((y)-1))
#endif

//��a������뵽4K����С������
#ifndef ACG_UPROUND4K
#define ACG_UPROUND4K(a) ACG_UPROUND(a,4096)
#endif

#ifndef ACG_UNUSE
#define ACG_UNUSE(a) a;
#endif

//���õȴ��߳̾���Լ���ȷ��ʱһ���ĺ�����,�����������ȷ��ϵͳ����Sleep
#define ACG_SLEEP(dwMilliseconds) WaitForSingleObject(GetCurrentThread(),dwMilliseconds)

//�������ȫ���н������ѭ���궨��
#define ACG_FOREVERLOOP for(;;)          

//���ƾ���ĺ궨��
//����һ��������̳б�־�ر�
#define ACG_DUPHANDLE(h,hDup) DuplicateHandle(GetCurrentProcess(),(h),GetCurrentProcess(),&(hDup),0,FALSE,DUPLICATE_SAME_ACCESS)
//����һ��������̳б�־��
#define ACG_DUPHANDLEI(h,hDup) DuplicateHandle(GetCurrentProcess(),(h),GetCurrentProcess(),&(hDup),0,TRUE,DUPLICATE_SAME_ACCESS)
//��һ���������Ϊ���ܼ̳�
#define ACG_SETHANDLE_NI(h) SetHandleInformation(h,HANDLE_FLAG_INHERIT,0)
//��һ���������Ϊ������CloseHandle�ر�
#define ACG_SETHANDLE_NC(h) SetHandleInformation(h,HANDLE_FLAG_PROTECT_FROM_CLOSE,HANDLE_FLAG_PROTECT_FROM_CLOSE)
//��һ���������Ϊ�ܼ̳�
#define ACG_SETHANDLE_I(h) SetHandleInformation(h,HANDLE_FLAG_INHERIT,HANDLE_FLAG_INHERIT)
//��һ���������Ϊ�ܹ���CloseHandle�ر�
#define ACG_SETHANDLE_C(h) SetHandleInformation(h,HANDLE_FLAG_PROTECT_FROM_CLOSE,0)
//=======================================����========================================