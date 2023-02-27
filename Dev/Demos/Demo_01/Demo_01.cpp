// Demo_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#ifdef _WIN32
#   ifdef _DEBUG
#       include <vld.h>
#   endif
#endif

#include "behaviac/behaviac.h"

#if BEHAVIAC_CCDEFINE_ANDROID
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "tutorial_3", __VA_ARGS__))
#else
#   define LOGI printf
#   if BEHAVIAC_CCDEFINE_MSVC
#       include <windows.h>
#       include <tchar.h>
#   endif
#endif


int main(int argc, char** argv)
{
    BEHAVIAC_UNUSED_VAR(argc);
    BEHAVIAC_UNUSED_VAR(argv);

    std::cout << "Hello World!\n";

    return 0;
}
