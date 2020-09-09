#include "stdafx.h"
#include "MySqlService.h"
#include <Winsvc.h>

using namespace fsutility;

CMySqlService::CMySqlService(void)
{
}


CMySqlService::~CMySqlService(void)
{
}

BOOL CMySqlService::StartMySqlService(CString strMysqlName)
{
	CString strServiceName(strMysqlName);
	SERVICE_STATUS_PROCESS ssStatus; 
	DWORD dwOldCheckPoint; 
	DWORD dwStartTickCount;
	DWORD dwWaitTime;
	DWORD dwBytesNeeded;

	SC_HANDLE  schSCManager = OpenSCManager( nullptr, nullptr, SC_MANAGER_ALL_ACCESS);  
	if (nullptr == schSCManager) 
	{
		return FALSE;
	}

	SC_HANDLE   schService = OpenService( 
		schSCManager,   
		strServiceName,    
		SERVICE_ALL_ACCESS);  

	if (schService == nullptr)
	{ 
		CloseServiceHandle(schSCManager);
		return FALSE;
	}    

	if (!QueryServiceStatusEx( 
		schService,        
		SC_STATUS_PROCESS_INFO,     
		(LPBYTE) &ssStatus,           
		sizeof(SERVICE_STATUS_PROCESS), 
		&dwBytesNeeded ) )   
	{
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return FALSE; 
	}

	if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
	{
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return (ssStatus.dwCurrentState == SERVICE_RUNNING); 
	}

	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
	{
		dwWaitTime = ssStatus.dwWaitHint / 10;

		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep( dwWaitTime );

		if (!QueryServiceStatusEx( 
			schService,             
			SC_STATUS_PROCESS_INFO,
			(LPBYTE) &ssStatus,  
			sizeof(SERVICE_STATUS_PROCESS), 
			&dwBytesNeeded ) )  
		{
			CloseServiceHandle(schService); 
			CloseServiceHandle(schSCManager);
			return FALSE; 
		}

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
			{
				printf("Timeout waiting for service to stop\n");
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager);
				return FALSE; 
			}
		}
	}

	if (!StartService(schService,  0,      nullptr) ) 
	{
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return FALSE; 
	}

	if (!QueryServiceStatusEx( 
		schService,       
		SC_STATUS_PROCESS_INFO,    
		(LPBYTE) &ssStatus,    
		sizeof(SERVICE_STATUS_PROCESS), 
		&dwBytesNeeded ) )  
	{
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return FALSE; 
	}

	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	while (ssStatus.dwCurrentState == SERVICE_START_PENDING) 
	{ 
		dwWaitTime = ssStatus.dwWaitHint / 10;

		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep( dwWaitTime );

		if (!QueryServiceStatusEx( 
			schService,           
			SC_STATUS_PROCESS_INFO, 
			(LPBYTE) &ssStatus,    
			sizeof(SERVICE_STATUS_PROCESS), 
			&dwBytesNeeded ) )   
		{
			break; 
		}

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
			{
				break;
			}
		}
	} 

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
	return (ssStatus.dwCurrentState == SERVICE_RUNNING) ;
}