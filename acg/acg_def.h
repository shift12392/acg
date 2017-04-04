
//一些常用宏定义
#pragma once
#define ACG_DEF_H


//用于输出编译信息时，将其他类型的信息变成字符串型的
#define ACG_STRING2(x) #x
#define ACG_STRING(x) ACG_STRING2(x)

#define ACG_WSTRING2(x) L #x
#define ACG_WSTRING(x) ACG_WSTRING2(x)

//__FILE__预定义宏的宽字符版本
#define ACG_WIDEN2(x) L ## x
#define ACG_WIDEN(x) ACG_WIDEN2(x)
#define __WFILE__ ACG_WIDEN(__FILE__)

//TLS 存储支持
#define ACG_TLS				__declspec( thread )

//内联定义
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

//检查安全字符串函数返回值，如果失败，则杀死进程
#define ACG_CHECK_STR(ret) if(S_OK!=(ret)) exit(0);

//安全delete
#ifndef ACG_SAFEDEL
#define ACG_SAFEDEL(p) if(NULL!=(p)) delete (p);
#endif

//取静态数组元素的个数
#ifndef ACG_COUNTOF
#define ACG_COUNTOF(a)		sizeof(a)/sizeof(a[0])
#endif

//将除法结果向上取整，如3/2 = 2
#ifndef ACG_UPDIV
#define ACG_UPDIV(a,b) ((a) + (b) - 1)/(b)
#endif

//将a对齐到b的整数倍
#ifndef ACG_UPROUND
#define ACG_UPROUND(x,y) (((x)+((y)-1))&~((y)-1))
#endif

//将a增大对齐到4K的最小整数倍
#ifndef ACG_UPROUND4K
#define ACG_UPROUND4K(a) ACG_UPROUND(a,4096)
#endif

#ifndef ACG_UNUSE
#define ACG_UNUSE(a) a;
#endif

//利用等待线程句柄自己精确延时一定的毫秒数,用于替代不精确的系统函数Sleep
#define ACG_SLEEP(dwMilliseconds) WaitForSingleObject(GetCurrentThread(),dwMilliseconds)

//《代码大全》中建议的死循环宏定义
#define ACG_FOREVERLOOP for(;;)          

//复制句柄的宏定义
//复制一个句柄，继承标志关闭
#define ACG_DUPHANDLE(h,hDup) DuplicateHandle(GetCurrentProcess(),(h),GetCurrentProcess(),&(hDup),0,FALSE,DUPLICATE_SAME_ACCESS)
//复制一个句柄，继承标志打开
#define ACG_DUPHANDLEI(h,hDup) DuplicateHandle(GetCurrentProcess(),(h),GetCurrentProcess(),&(hDup),0,TRUE,DUPLICATE_SAME_ACCESS)
//将一个句柄设置为不能继承
#define ACG_SETHANDLE_NI(h) SetHandleInformation(h,HANDLE_FLAG_INHERIT,0)
//将一个句柄设置为不能用CloseHandle关闭
#define ACG_SETHANDLE_NC(h) SetHandleInformation(h,HANDLE_FLAG_PROTECT_FROM_CLOSE,HANDLE_FLAG_PROTECT_FROM_CLOSE)
//将一个句柄设置为能继承
#define ACG_SETHANDLE_I(h) SetHandleInformation(h,HANDLE_FLAG_INHERIT,HANDLE_FLAG_INHERIT)
//将一个句柄设置为能够用CloseHandle关闭
#define ACG_SETHANDLE_C(h) SetHandleInformation(h,HANDLE_FLAG_PROTECT_FROM_CLOSE,0)
//=======================================结束========================================