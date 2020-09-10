#include "StdAfx.h"
#include "SysPathManager.h"

using namespace fsutility;

CSysPathManager::CSysPathManager(void)
{
	TCHAR lpBuffer[1024] ;
	int nBufferSize = sizeof(lpBuffer) / sizeof(TCHAR) ;
	::GetModuleFileName(nullptr, lpBuffer, 1024);
	m_strExePath.Format(_T("%s"), lpBuffer);
	int nTemp = m_strExePath.ReverseFind(_T('\\')) ;
	m_strExePath.Delete(nTemp+1, m_strExePath.GetLength() - nTemp - 1) ;	

	m_strConfigPath = m_strExePath + CString(_T("config\\"));

	m_strAffineFilePath = m_strExePath + CString(_T("config\\TransformParameters.txt"));

	m_strLogPath = m_strExePath + CString(_T("Log\\"));

	m_strSVMPath = m_strExePath + CString(_T("SVM\\"));
}

CSysPathManager::~CSysPathManager(void)
{
}


CString  CSysPathManager::GetExePath()
{
	return m_strExePath;
}

CString  CSysPathManager::GetConfigPath()
{
	return m_strConfigPath;
}

CString  CSysPathManager::GetLogPath()
{
	return m_strLogPath;
}

CString  CSysPathManager::GetAffineFilePath()
{
	return m_strAffineFilePath;
}

CString  CSysPathManager::GetSVMPath()
{
	return m_strSVMPath;
}