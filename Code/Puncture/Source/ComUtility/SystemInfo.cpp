#include "StdAfx.h"
#include "SystemInfo.h"
using namespace fsutility;
SystemInfo::SystemInfo(void)
{
}

SystemInfo::~SystemInfo(void)
{
}

int SystemInfo::GetCPUProcessorNum()
{
	SYSTEM_INFO sysInfo;
	::GetSystemInfo(&sysInfo);
	DWORD dwNumCpu = sysInfo.dwNumberOfProcessors;
	return dwNumCpu;
}
