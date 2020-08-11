#pragma once

namespace fsutility
{
class AFX_EXT_CLASS CFSEnableProcessPriority
{
public:
	CFSEnableProcessPriority(void);
	~CFSEnableProcessPriority(void);

	//设置当前进程优先级为最高(实时)
	//
	BOOL SetRealTimePriority();

	BOOL SetHighPriority();

	//
	//提升当前进程权限函数("SeShutdownPrivilege"关机权限)
	//
	BOOL EnableShutdownPriv();

	//
	//提升当前进程权限函数("SeDebugPrivilege"读、写控制权限)
	//
	BOOL EnableDebugPriv();

	//
	//提升当前进程权限函数("SeBackupPrivilege"注册表备份权限)
	//
	BOOL EnableBackupPriv();

	//
	//提升当前进程权限函数("SeRestorePrivilege"恢复数据权限)
	//
	BOOL EnableRestorePriv();
};

}
