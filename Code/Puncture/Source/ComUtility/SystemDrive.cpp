#include "stdafx.h"
#include "SystemDrive.h"

using namespace  fsutility;

SystemDrive::SystemDrive(void)
{
	TCHAR szBuf[100];
	memset(szBuf,0,100);
	DWORD len = GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);
	for (TCHAR* s = szBuf; *s; s += _tcslen(s)+1)
	{
		LPCTSTR sDrivePath = s;
		DriverList.push_back(sDrivePath);
	}
}


SystemDrive::~SystemDrive(void)
{
}

BOOL    SystemDrive::IsCDDriver(CString strDriver)
{
	UINT   Type = GetDriveType(strDriver); ;   
	if (Type==DRIVE_CDROM)     
	{
		return TRUE;
	}
	return FALSE;
}


CString SystemDrive::GetCDDriver()
{
	for (auto driver: DriverList)
	{
		UINT   Type = GetDriveType(driver); ;   
		if (Type==DRIVE_CDROM)     
		{
			return driver;
		}
	}
	return _T("");
}

BOOL    SystemDrive::IsCDInDriver(CString strDriver)
{
	UINT   Type = GetDriveType(strDriver); ;   
	if (Type==DRIVE_CDROM)     
	{
		DWORD dwSize = 0;
		GetDiskFreeSpace(strDriver, nullptr, nullptr, nullptr, &dwSize);
		if(dwSize > 0)
			return TRUE;
	}
	return FALSE;
}
