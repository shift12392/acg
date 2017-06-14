#pragma once



//文件描述：每个动态库都要使用的标准头文件 动态库中包含时 预定义GRS_EXPORT宏

//先取消ACG_DLL定义
#ifdef ACG_DLL
#undef ACG_DLL
#endif // ACG_DLL

//-----------------------------------------------------------------------------------
//注意这个文件中没有使用防止重新宏包含的特性，主要是为了方便在多个不同的模块中，
//利用重定义ACG_EXPORT宏的方式灵活的定义导出 导入的情形
//-----------------------------------------------------------------------------------

#ifdef ACG_EXPORT

#define ACG_DLL __declspec( dllexport )

#else

#define ACG_DLL __declspec( dllimport )

#endif // ACG_EXPORT


//取消对ACG_EXPORT的定义
#ifdef ACG_EXPORT
#undef ACG_EXPORT
#endif // ACG_EXPORT
