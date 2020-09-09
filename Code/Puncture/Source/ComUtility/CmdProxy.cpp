#include "StdAfx.h"
#include "CmdProxy.h"
#include "FsStringConvert.h"

using namespace fsutility;

CmdProxy::CmdProxy(void)
{
}

CmdProxy::~CmdProxy(void)
{
}


BOOL CmdProxy::RunCmd(CString strCmdText, CString &strResult)
{
	//构建运行命令
	CString strCmd;
	strCmd.Format(_T("cmd /c %s"),strCmdText);

	/*通过匿名管道方式获取cmd命令运行输出结果*/
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	//使用系统默认的安全描述符 
	sa.lpSecurityDescriptor = nullptr;  
	//创建的进程继承句柄
	sa.bInheritHandle = TRUE;        

	if (!CreatePipe(&hRead,&hWrite,&sa,0)) 
	{  
		return FALSE;
	}

	STARTUPINFO si; 
	PROCESS_INFORMATION pi;

	ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO); 

	GetStartupInfo(&si); 

	 //新创建进程的标准输出连在写管道一端
	si.hStdError = hWrite; 
	si.hStdOutput = hWrite; 
	si.wShowWindow = SW_HIDE; 

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcess(nullptr,strCmd.GetBuffer(0),nullptr,nullptr,TRUE,NULL,nullptr,nullptr,&si,&pi))  //创建子进程
	{
		CloseHandle(hWrite); 
		CloseHandle(hRead); 
		return FALSE;
	}

	//等待cmd命令运行完成，最长等待60s
	WaitForSingleObject(pi.hProcess, 30000);
	CloseHandle(hWrite); 

	TCHAR buffer[4096] = {0};
	DWORD bytesRead=0;
	while (true) 
	{
		if (ReadFile(hRead,buffer,4095,&bytesRead,nullptr) == NULL)  //读取管道
			break;
		buffer[bytesRead] = _T('\0');
		strResult += buffer;
	}

	CloseHandle(hRead);
	CloseHandle(pi.hThread);   
	CloseHandle(pi.hProcess);

	return TRUE;
}