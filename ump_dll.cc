#include "ump_pipeline.h"
#include "ump_dll.h"

IUmpContext* CreateMediapipeContext()
{
	return UmpCreateContext();
}

#if defined(_WIN32)
	#include <windows.h>
	BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) { return TRUE; }
#endif
