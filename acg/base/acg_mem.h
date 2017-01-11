#pragma once


#define ACG_ALLOC(sz)		HeapAlloc(GetProcessHeap(),0,sz)
#define ACG_CALLOC(sz)		HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sz)
#define ACG_REALLOC(p,sz)	HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,p,sz)
#define ACG_FREE(p)			HeapFree(GetProcessHeap(),0,p)
#define ACG_MSIZE(p)		HeapSize(GetProcessHeap(),0,p)
#define ACG_MVALID(p)		HeapValidate(GetProcessHeap(),0,p)

//下面这个表达式可能会引起副作用,p参数尽量使用指针变量,而不是表达式
#define ACG_SAFEFREE(p) if(NULL != (p)){HeapFree(GetProcessHeap(),0,(p));(p)=NULL;}

//下面这个宏用于打开堆的LFH特性,以提高性能
#define ACG_OPEN_HEAP_LFH(h) \
    ULONG  ulLFH = 2;\
    HeapSetInformation((h),HeapCompatibilityInformation,&ulLFH ,sizeof(ULONG) ) ;