#include "StdAfx.h"
#include "GraphicsCardEnum.h"
#include <stdio.h>
#include <stdlib.h>
#include <cfgmgr32.h> 
#include <Dbt.h>

#pragma comment(lib, "setupapi.lib")

using namespace fsutility;

CGraphicsCardEnum::CGraphicsCardEnum(void)
{
}

CGraphicsCardEnum::~CGraphicsCardEnum(void)
{
}

BOOL CGraphicsCardEnum::GetGraphicsCardString(std::vector<CString> &Card)
{
	Card.clear();
	USES_CONVERSION;
	SP_DEVINFO_DATA    DeviceInfoData    =    {sizeof(SP_DEVINFO_DATA)}; 
	DWORD        BufSize                =    MAX_PATH;     
	DWORD        RetBufSize            =    0;
	DWORD        DeviceId            =    0;
	TCHAR        szDevName[MAX_PATH]    ;

	TCHAR        szDesc[MAX_PATH];    
	HDEVINFO    hDevInfo;
	DWORD        Status, Problem; 

	// 返回所有设备信息

	hDevInfo = SetupDiGetClassDevs(nullptr,nullptr,0,DIGCF_PRESENT|DIGCF_ALLCLASSES) ; 

	if (INVALID_HANDLE_VALUE == hDevInfo ) 
	{
		return FALSE; 
	}

	// 枚举设备
	for (DeviceId=0;SetupDiEnumDeviceInfo( hDevInfo,DeviceId,&DeviceInfoData);DeviceId++) 
	{ 
		// 获得设备的状态
		if (CM_Get_DevNode_Status(&Status, &Problem, DeviceInfoData.DevInst ,0) != CR_SUCCESS) 
		{
			continue; 
		}

		RtlZeroMemory(szDevName,MAX_PATH);
		// 获取设备类名
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData,SPDRP_CLASS ,nullptr,(PBYTE)szDevName,BufSize,&RetBufSize)
			//&& CGraphicsCardEnum::IsDeviceDisabled(DeviceId, hDevInfo)
			)
		{
			CString strDevice = szDevName;
			if (strDevice.MakeLower() == _T("display")) 
			{
				RtlZeroMemory(szDesc,MAX_PATH);
				// 获取设备描述                    
				if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC,nullptr,(PBYTE)szDesc,BufSize,&RetBufSize)) 
				{
					Card.push_back((CString)szDesc);
				}
			}
		}
	} 
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return TRUE; 
}

BOOL CGraphicsCardEnum::IsDeviceDisabled(DWORD dwDevID, HDEVINFO hDevInfo)
{
	SP_DEVINFO_DATA DevInfoData = {sizeof(SP_DEVINFO_DATA)};
	DWORD    dwDevStatus,dwProblem;
	if(!SetupDiEnumDeviceInfo(hDevInfo,dwDevID,&DevInfoData))
	{
		return FALSE;
	}

	//查询设备状态
	if(CM_Get_DevNode_Status(&dwDevStatus,&dwProblem,DevInfoData.DevInst,0)!=CR_SUCCESS)
	{
		return FALSE;
	}
	return ((dwDevStatus & DN_HAS_PROBLEM) && (dwProblem == CM_PROB_DISABLED));
}