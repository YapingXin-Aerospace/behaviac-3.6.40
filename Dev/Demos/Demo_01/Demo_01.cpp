// Demo_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#ifdef _WIN32
#   ifdef _DEBUG
#       include <vld.h>
#   endif
#endif

#include "behaviac/behaviac.h"
#include "behaviac/behaviac_generated/types/behaviac_types.h"


#define LOGI printf
#if BEHAVIAC_CCDEFINE_MSVC
#include <windows.h>
#include <tchar.h>
#endif


static void SetExePath();
static void Run();
static bool InitBehavic(behaviac::Workspace::EFileFormat ff);
static bool InitPlayer();
static void UpdateLoop();
static void CleanupPlayer();
static void CleanupBehaviac();

static FirstAgent* g_FirstAgent = NULL;


int main(int argc, char** argv)
{
    BEHAVIAC_UNUSED_VAR(argc);
    BEHAVIAC_UNUSED_VAR(argv);

	SetExePath();

	LOGI("BEHAVIAC_CCDEFINE_NAME=%s\n", BEHAVIAC_CCDEFINE_NAME);

	Run();

	// printf("Press any key to continue...");
	// int ret = getchar();
	// BEHAVIAC_UNUSED_VAR(ret);

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


static void Run()
{
	const char* szTreeName = "Demo_01";
	LOGI("bt: %s\n\n", szTreeName);

	behaviac::Workspace::EFileFormat ff = behaviac::Workspace::EFF_xml;

	InitBehavic(ff);
	InitPlayer();
	UpdateLoop();
	CleanupPlayer();
	CleanupBehaviac();
}


static bool InitBehavic(behaviac::Workspace::EFileFormat ff)
{
	const char* szFilePath = "../../../../../../../Demos/Demo_01/behaviac/exported";

	LOGI("InitBehavic\n");

	behaviac::Workspace::GetInstance()->SetFilePath(szFilePath);
	behaviac::Workspace::GetInstance()->SetFileFormat(ff);

	return true;
}


static bool InitPlayer()
{
	LOGI("InitPlayer\n");

	g_FirstAgent = behaviac::Agent::Create<FirstAgent>();

	bool bRet = g_FirstAgent->btload("FirstBT");
	g_FirstAgent->btsetcurrent("FirstBT");
	return bRet;
}


static void UpdateLoop()
{
	LOGI("UpdateLoop\n");

	int frames = 0;
	behaviac::EBTStatus status = behaviac::BT_RUNNING;

	while (status == behaviac::BT_RUNNING)
	{
		LOGI("frame %d\n", ++frames);

		status = g_FirstAgent->btexec();
	}
}


static void CleanupPlayer()
{
	LOGI("CleanupPlayer\n");

	behaviac::Agent::Destroy(g_FirstAgent);
}


static void CleanupBehaviac()
{
	LOGI("CleanupBehaviac\n");

	behaviac::Workspace::GetInstance()->Cleanup();
}
