// Demo_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#ifdef _WIN32
#   ifdef _DEBUG
#       include <vld.h>
#   endif
#endif

#include "behaviac/behaviac.h"


#define LOGI printf
#if BEHAVIAC_CCDEFINE_MSVC
#include <windows.h>
#include <tchar.h>
#endif


static void SetExePath();


int main(int argc, char** argv)
{
    BEHAVIAC_UNUSED_VAR(argc);
    BEHAVIAC_UNUSED_VAR(argv);

	SetExePath();

	LOGI("BEHAVIAC_CCDEFINE_NAME=%s\n", BEHAVIAC_CCDEFINE_NAME);

    std::cout << "Hello World!\n";

	printf("Press any key to continue...");
	int ret = getchar();
	BEHAVIAC_UNUSED_VAR(ret);

    return 0;
}


static void SetExePath()
{
	TCHAR szCurPath[_MAX_PATH];

	GetModuleFileName(NULL, szCurPath, _MAX_PATH);

	TCHAR* p = szCurPath;

	while (_tcschr(p, L'\\'))
	{
		p = _tcschr(p, L'\\');
		p++;
	}

	*p = L'\0';

	SetCurrentDirectory(szCurPath);
}
