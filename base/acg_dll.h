#pragma once



//�ļ�������ÿ����̬�ⶼҪʹ�õı�׼ͷ�ļ� ��̬���а���ʱ Ԥ����GRS_EXPORT��

//��ȡ��ACG_DLL����
#ifdef ACG_DLL
#undef ACG_DLL
#endif // ACG_DLL

//-----------------------------------------------------------------------------------
//ע������ļ���û��ʹ�÷�ֹ���º���������ԣ���Ҫ��Ϊ�˷����ڶ����ͬ��ģ���У�
//�����ض���ACG_EXPORT��ķ�ʽ���Ķ��嵼�� ���������
//-----------------------------------------------------------------------------------

#ifdef ACG_EXPORT

#define ACG_DLL __declspec( dllexport )

#else

#define ACG_DLL __declspec( dllimport )

#endif // ACG_EXPORT


//ȡ����ACG_EXPORT�Ķ���
#ifdef ACG_EXPORT
#undef ACG_EXPORT
#endif // ACG_EXPORT
