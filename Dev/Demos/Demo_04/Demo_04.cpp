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
static bool InitPlayer(const char* btName);
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
	const char* szTreeName = "Demo_04";
	LOGI("bt: %s\n\n", szTreeName);

	behaviac::Workspace::EFileFormat ff = behaviac::Workspace::EFF_xml;

	InitBehavic(ff);
	
	bool bInit = InitPlayer("maintree_task");
	if (bInit)
	{
		UpdateLoop();
		CleanupPlayer();
	}

	CleanupBehaviac();
}


static bool InitBehavic(behaviac::Workspace::EFileFormat ff)
{
	const char* szFilePath = "../../../../../../../Demos/Demo_04/behaviac/exported";

	LOGI("InitBehavic\n");

	behaviac::Workspace::GetInstance()->SetFilePath(szFilePath);
	behaviac::Workspace::GetInstance()->SetFileFormat(ff);

	return true;
}


static bool InitPlayer(const char* btName)
{
	LOGI("InitPlayer : %s\n", btName);

	g_FirstAgent = behaviac::Agent::Create<FirstAgent>();

	bool bRet = g_FirstAgent->btload(btName);
	g_FirstAgent->btsetcurrent(btName);
	return bRet;
}


static void UpdateLoop()
{
	LOGI("UpdateLoop\n");

	behaviac::EBTStatus status = g_FirstAgent->btexec();
	BEHAVIAC_ASSERT(status == behaviac::BT_RUNNING);
	BEHAVIAC_UNUSED_VAR(status);

	g_FirstAgent->FireEvent("event_task", 2);
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
