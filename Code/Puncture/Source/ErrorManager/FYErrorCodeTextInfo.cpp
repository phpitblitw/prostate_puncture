#include "stdafx.h"
#include "FYErrorCodeTextInfo.h"


FYErrorCodeTextInfo::FYErrorCodeTextInfo(void)
{
}


FYErrorCodeTextInfo::~FYErrorCodeTextInfo(void)
{
}


BOOL   FYErrorCodeTextInfo::LoadErrorCodeFile(CString strFileName)
{
	return m_ErrorCodeConfig.Open(strFileName);
}

CString FYErrorCodeTextInfo::GetCodeErrorInfo(DWORD nErrorCode)
{
	CString strCode;
	strCode.Format(_T("0x%08X"), nErrorCode);
	CString strInfo;
	m_ErrorCodeConfig.Move2Section(_T("error_code"));
	m_ErrorCodeConfig.ReadKey(strCode, strInfo);
	return strInfo;

}	