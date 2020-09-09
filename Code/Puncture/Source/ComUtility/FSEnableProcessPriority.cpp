#include "StdAfx.h"
#include "FSEnableProcessPriority.h"

using namespace fsutility;

CFSEnableProcessPriority::CFSEnableProcessPriority(void)
{
}

CFSEnableProcessPriority::~CFSEnableProcessPriority(void)
{
}

//宏的功能介绍:
//-------------------------------------------------------------
//#define            SE_BACKUP_NAME               TEXT("SeBackupPrivilege")     //备份数据权限
//#define            SE_RESTORE_NAME              TEXT("SeRestorePrivilege")    //恢复数据权限
//#define            SE_SHUTDOWN_NAME             TEXT("SeShutdownPrivilege")   //关机权限
//#define            SE_DEBUG_NAME                TEXT("SeDebugPrivilege")      //读、写控制权限
//-------------------------------------------------------------

//宏的功能介绍:
//-------------------------------------------------------------
//ABOVE_NORMAL_PRIORITY_CLASS(0x00008000)   高于标准
//BELOW_NORMAL_PRIORITY_CLASS(0x00004000)   低于标准
//HIGH_PRIORITY_CLASS(0x00000080)           高
//IDLE_PRIORITY_CLASS(0x00000040)           低
//NORMAL_PRIORITY_CLASS(0x00000020)         标准
//REALTIME_PRIORITY_CLASS(0x00000100)       实时
//-------------------------------------------------------------

//
//设置当前进程优先级为最高(实时)
//
BOOL CFSEnableProcessPriority::SetRealTimePriority()
{
	if( ! SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS ))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CFSEnableProcessPriority::SetHighPriority()
{
	if( ! SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS ))
	{
		return FALSE;
	}

	return TRUE;
}


//
//提升当前进程权限函数("SeShutdownPrivilege"关机权限)
//
BOOL CFSEnableProcessPriority::EnableShutdownPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return FALSE;

	if ( ! LookupPrivilegeValue( nullptr, SE_SHUTDOWN_NAME, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, nullptr, nullptr ) )
		CloseHandle( hToken );

	return TRUE;
}

//
//提升当前进程权限函数("SeDebugPrivilege"读、写控制权限)
//
BOOL CFSEnableProcessPriority::EnableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return FALSE;

	if ( ! LookupPrivilegeValue( nullptr, SE_DEBUG_NAME, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, nullptr, nullptr ) )
		CloseHandle( hToken );

	return TRUE;
}

//
//提升当前进程权限函数("SeBackupPrivilege"备份数据权限)
//
BOOL CFSEnableProcessPriority::EnableBackupPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return FALSE;

	if ( ! LookupPrivilegeValue( nullptr, SE_BACKUP_NAME, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, nullptr, nullptr ) )
		CloseHandle( hToken );

	return TRUE;
}

//
//提升当前进程权限函数("SeRestorePrivilege"恢复数据权限)
//
BOOL CFSEnableProcessPriority::EnableRestorePriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return FALSE;

	if ( ! LookupPrivilegeValue( nullptr, SE_RESTORE_NAME, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, nullptr, nullptr ) )
		CloseHandle( hToken );

	return TRUE;
}