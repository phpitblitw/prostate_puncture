#include "StdAfx.h"
#include "PathUtility.h"
#include <shlwapi.h>
#include "shlwapi.h"
#pragma comment(lib, "Shlwapi")

using namespace fsutility;

PathUtility::PathUtility(void)
{
}


PathUtility::~PathUtility(void)
{
}

BOOL   PathUtility::DirExists(LPCTSTR   lpszDir)  
{  
	DWORD attr;  
	attr   =   GetFileAttributes(lpszDir);  
	return   attr   !=   (DWORD)-1   &&   attr   &   FILE_ATTRIBUTE_DIRECTORY;  
}  


BOOL  PathUtility::DeleteFolder( LPCTSTR pszFolder)
{
	if(!DirExists(pszFolder))
		return TRUE; 

	CString strDir, strFile;
	strDir = pszFolder;
	
	if( strDir.Right(1) != _T("\\") )
	{
		strDir += _T("\\");
	}
	strDir += _T("*.*");
	
	CFileFind tempFind; 
	BOOL IsFinded=tempFind.FindFile(strDir); 

	while(IsFinded) 
	{ 
		IsFinded=tempFind.FindNextFile(); 
		if ( tempFind.IsDots() )
			continue;

		CString strFileName = tempFind.GetFileName();
		strFile = pszFolder;
		
		if ( strFile.Right(1) != _T("\\") )
		{
			strFile += _T("\\");
		}

		strFile += strFileName;

		if ( tempFind.IsDirectory())
		{
			DeleteFolder(strFile.GetBuffer(0));
		}
		else
		{
			if (tempFind.IsReadOnly())
			{
				SetFileAttributes(strFile, GetFileAttributes(strFile) &	(~FILE_ATTRIBUTE_READONLY) );
			}
			DeleteFile(strFile);
		}
	}

	tempFind.Close(); 
	if(!RemoveDirectory(pszFolder)) 
	{ 
		return FALSE; 
	} 
	return TRUE;
}

CString PathUtility::GetParentPath(const CString &strPath)
{
	CString strParentPath = strPath ;
	if(strParentPath.Right(1) == _T("\\"))
	{
		strParentPath.TrimRight(_T("\\"));
	}
	int pos = strParentPath.ReverseFind(_T('\\'));
	//增加该判断时为了对根目录进行处理
	if(pos>2)
	{
		strParentPath = strParentPath.Left(pos);
	}
	return strParentPath ; 
}

void PathUtility::FindAllSubDir(const CString& strPath, 
	CList<CString>& listDir,
	BOOL bIncludeParentPath /*= TRUE*/)
{
	CFileFind ff;
	BOOL bOK = ff.FindFile(strPath + _T("\\*.*"));

	while (bOK)
	{
		bOK = ff.FindNextFile();
		if (ff.IsDots())
			continue;

		if (ff.IsDirectory())
		{
			bIncludeParentPath 
				? listDir.AddTail(ff.GetFilePath())
				: listDir.AddTail(ff.GetFileName());
		}
	}
}


void PathUtility::FindAllFileinPath(const CString& strPath, 
	CList<CString>& listFileName,
	BOOL bIncludePathName /*= TRUE*/)
{
	CFileFind ff;
	BOOL bOK = ff.FindFile(strPath + _T("\\*.*"));

	while (bOK)
	{
		bOK = ff.FindNextFile();
		if (!ff.IsDots() && !ff.IsDirectory())
		{
			bIncludePathName 
				? listFileName.AddTail(ff.GetFilePath())
				: listFileName.AddTail(ff.GetFileName());
		}
	}
}

BOOL PathUtility::IsFileinPath(const CString& strFile2Find,
	const CString& strPath)
{
	CFileFind ff;
	BOOL bOK = ff.FindFile(strPath + _T("\\*.*"));

	while (bOK)
	{
		bOK = ff.FindNextFile();
		if (!ff.IsDots())
		{
			if (ff.GetFileName() == strFile2Find)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL PathUtility::RecurRMDir(const CString& strDir)
{
	if (strDir.IsEmpty())
	{
		return TRUE;
	}

	if (!IsDir(strDir))
	{
		return FALSE;
	}

	CList<CString> listSubDir;
	FindAllSubDir(strDir, listSubDir);
	{
		POSITION pos = listSubDir.GetHeadPosition();
		while (pos != nullptr)
		{
			const CString& strSubDir = listSubDir.GetNext(pos);
			if (!RecurRMDir(strSubDir))
			{
				return FALSE;
			}
		}
	}

	CList<CString> listFiles;
	FindAllFileinPath(strDir, listFiles);
	{
		POSITION pos = listFiles.GetHeadPosition();
		while (pos != nullptr)
		{
			const CString& strFilePath = listFiles.GetNext(pos);
			CFile::Remove(strFilePath);
		}
	}
	RemoveDirectory(strDir);

	return TRUE;
}


BOOL PathUtility::RecurMakeDir(const CString& strFullPath)
{
	if (PathFileExists(strFullPath))
		return TRUE;

	if (strFullPath.IsEmpty())
		return FALSE;

	CString strTemp = strFullPath;

	if (strTemp.ReverseFind(_T('\\')) != strTemp.GetLength()-1)
	{
		strTemp.Append(_T("\\"));
	}
	int nPos = 0;
	while (TRUE)
	{
		nPos = strTemp.Find(_T('\\'), nPos+1);   
		if (nPos == -1)
			break;
		if (nPos <= 2)
			continue;

		::CreateDirectory(strTemp.Left(nPos+1), nullptr);
	}

	return PathFileExists(strFullPath);
}

BOOL PathUtility::RecurCopyDir(const CString& strFromDir, 
	const CString& strToDir)
{
	if (!PathFileExists(strFromDir))
		return FALSE;

	if (!RecurMakeDir(strToDir))
		return FALSE;

	CList<CString> listSubDir;
	FindAllSubDir(strFromDir, listSubDir, FALSE);
	{
		POSITION pos = listSubDir.GetHeadPosition();
		while (pos != nullptr)
		{
			const CString& strSubDir = listSubDir.GetNext(pos);
			RecurCopyDir(strFromDir + _T("\\") + strSubDir, strToDir + _T("\\") + strSubDir);
		}
	}

	CList<CString> listFiles;
	FindAllFileinPath(strFromDir, listFiles, FALSE);
	{
		POSITION pos = listFiles.GetHeadPosition();
		while (pos != nullptr)
		{
			const CString& strFileName= listFiles.GetNext(pos);
			::CopyFile(
				strFromDir + _T("\\") + strFileName, 
				strToDir + _T("\\") + strFileName,
				FALSE);
		}
	}

	return TRUE;
}

BOOL PathUtility::IsDir(const CString& strPath)
{
	DWORD dwAttr = ::GetFileAttributes(strPath);
	return dwAttr != INVALID_FILE_ATTRIBUTES 
		?  dwAttr & FILE_ATTRIBUTE_DIRECTORY
		:  FALSE;
}

BOOL PathUtility::IsPathFileExists(const CString& strPath)
{
	return ::PathFileExists(strPath.GetString());
}


CString PathUtility::ExtractFilePath(const CString& strPath)
{
	int nPos = strPath.ReverseFind(_T('\\'));
	if(nPos != -1)
	{
		return  strPath.Mid(0, nPos+1);
	}
	return strPath;
}