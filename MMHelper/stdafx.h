// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <sys/timeb.h>
#include <time.h>

#include "MMHelperHead.h"

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	#ifdef _DLL
	#ifdef _WIN64
		#pragma comment(lib, "../lib/svg64.lib")
	#else
		#pragma comment(lib, "../lib/svg.lib")
	#endif
	#else
	#ifdef _WIN64
		#pragma comment(lib, "../lib/svg64.lib")
	#else
		#pragma comment(lib, "../lib/svg.lib")
	#endif
	#endif
#else
	#ifdef _DLL
	#ifdef _WIN64
		#pragma comment(lib, "../lib/svg64.lib")
	#else
		#pragma comment(lib, "../lib/svg.lib")
	#endif
	#else
	#ifdef _WIN64
		#pragma comment(lib, "../lib/svg64.lib")
	#else
		#pragma comment(lib, "../lib/svg.lib")
	#endif
	#endif
#endif
