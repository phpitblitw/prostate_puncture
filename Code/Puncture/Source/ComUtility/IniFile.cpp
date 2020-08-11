#include "StdAfx.h"
#include "IniFile.h"
#include "FSStringHelper.h"


namespace fsutility
{
	CIniFile::CIniFile(const CString& strFileName)
		: m_pStrFileName(strFileName)
		, m_pStrSecName(_T(""))
		, m_nOpenFlags(modeReadWrite)
	{
	}

	CIniFile::CIniFile()
	{
	}

	BOOL CIniFile::Open(const CString& strFilePath,
		OpenMode mode)
	{
		m_pStrFileName = strFilePath;
		m_nOpenFlags = mode;
		return TRUE;
	}

	CIniFile::~CIniFile(void)
	{
	}

	void CIniFile::Move2Section(const CString& strSecName)
	{
		m_pStrSecName = strSecName;
	}

	BOOL CIniFile::WriteKey(const CString& strKeyName,
		unsigned int nVal) const 
	{
		return WriteKey(strKeyName, fsutility::FSStringHelper::ToCString(nVal));
	}

	BOOL CIniFile::WriteKey(const CString& strKeyName, 
		const CString& strValue) const 
	{
		return ::WritePrivateProfileString(m_pStrSecName, strKeyName, 
			strValue, m_pStrFileName);
	}

	BOOL CIniFile::WriteKey(const CString& strKeyName, 
		int nVal) const
	{
		return WriteKey(strKeyName, fsutility::FSStringHelper::ToCString(nVal));
	}

	BOOL CIniFile::WriteKey(const CString& strKeyName,
		float fVal) const
	{
		return WriteKey(strKeyName, fsutility::FSStringHelper::ToCString(fVal));
	}

	BOOL CIniFile::WriteKey(const CString& strKeyName,
		LPVOID lpStruct, 
		UINT uSizeStruct) const
	{
		return WritePrivateProfileStruct(m_pStrSecName, strKeyName, 
			lpStruct, uSizeStruct, m_pStrFileName);
	}

	BOOL CIniFile::WriteSection(const CString& strKey_Value) const
	{
		return ::WritePrivateProfileSection(m_pStrSecName, strKey_Value, m_pStrFileName);
	}

	void CIniFile::ReadKey(const CString& strKeyName, 
		CString& strVal) const
	{
		TCHAR TempStr[1000] = {0};
		const CString strDefault = strVal;
		::GetPrivateProfileStringW(m_pStrSecName, strKeyName, strDefault, TempStr, 1000, m_pStrFileName);
		strVal = TempStr;
	}

	void CIniFile::ReadKey(const CString& strKeyName, 
		int& nValInOut) const
	{
		nValInOut = ::GetPrivateProfileInt(m_pStrSecName, strKeyName, 
			nValInOut, m_pStrFileName);
	}

	void CIniFile::ReadKey(const CString& strKeyName, 
		unsigned int& nValInOut) const
	{
		nValInOut = ::GetPrivateProfileInt(m_pStrSecName, strKeyName, 
			nValInOut, m_pStrFileName);
	}

	void CIniFile::ReadKey(const CString& strKeyName, 
		DWORD& nValInOut) const
	{
		nValInOut = ::GetPrivateProfileInt(m_pStrSecName, strKeyName, 
			nValInOut, m_pStrFileName);
	}

	void CIniFile::ReadKey(const CString& strKeyName,
		float& fValInOut) const
	{
		TCHAR TempStr[1000] = _T("");

		::GetPrivateProfileString(m_pStrSecName, strKeyName, 
			FSStringHelper::ToCString(fValInOut), TempStr, 1000, m_pStrFileName);
		fValInOut = (float) _tstof(TempStr); 
	}

	void CIniFile::ReadKey(const CString& strKeyName,
		double& dValInOut) const
	{
		TCHAR TempStr[1000] = _T("");

		::GetPrivateProfileString(m_pStrSecName, strKeyName,
			FSStringHelper::ToCString(dValInOut), TempStr, 1000, m_pStrFileName);
		dValInOut = (double)_tstof(TempStr);
	}

	BOOL CIniFile::ReadKey(const CString& strKeyName,
		LPVOID lpStructOut, 
		UINT uSizeStruct) const
	{
		return ::GetPrivateProfileStruct(m_pStrSecName, strKeyName,
			lpStructOut, uSizeStruct, m_pStrFileName);
	}

	BOOL CIniFile::CanRead() const
	{
		return m_nOpenFlags & modeRead;
	}

	BOOL CIniFile::CanWrite() const
	{
		return m_nOpenFlags & modeWrite;
	}

	BOOL CIniFile::CanReadWrite() const
	{
		return m_nOpenFlags&modeRead && m_nOpenFlags&modeWrite;
	}

}
