#pragma once


#define ACG_ALLOC(sz)		HeapAlloc(GetProcessHeap(),0,sz)
#define ACG_CALLOC(sz)		HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sz)
#define ACG_REALLOC(p,sz)	HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,p,sz)
#define ACG_FREE(p)			HeapFree(GetProcessHeap(),0,p)
#define ACG_MSIZE(p)		HeapSize(GetProcessHeap(),0,p)
#define ACG_MVALID(p)		HeapValidate(GetProcessHeap(),0,p)

//����������ʽ���ܻ���������,p��������ʹ��ָ�����,�����Ǳ��ʽ
#define ACG_SAFEFREE(p) if(NULL != (p)){HeapFree(GetProcessHeap(),0,(p));(p)=NULL;}

//������������ڴ򿪶ѵ�LFH����,���������
#define ACG_OPEN_HEAP_LFH(h) \
    ULONG  ulLFH = 2;\
    HeapSetInformation((h),HeapCompatibilityInformation,&ulLFH ,sizeof(ULONG) ) ;