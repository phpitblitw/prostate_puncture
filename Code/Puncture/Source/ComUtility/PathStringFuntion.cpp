#include "StdAfx.h"
#include "PathStringFuntion.h"

using namespace fsutility;

CPathStringFunction::CPathStringFunction(void)
{
}

CPathStringFunction::~CPathStringFunction(void)
{
}


CString CPathStringFunction::TrimFileNameFromFullPath(CString strFullPath)
{
	int nFindPos = strFullPath.ReverseFind(_T('\\'));
	if(nFindPos != -1)
	{
		return strFullPath.Left(nFindPos+1);
	}
	return strFullPath;
}

//检查路径是否存在，如果不存在则创建
//TRUE   存在目录
//FALSE  不存在目录，且创建该失败
BOOL  CPathStringFunction::CheckAndCreatePath(CString strDirectory)
{
	DWORD   dwAttr  = GetFileAttributes(strDirectory);
	if(dwAttr==-1||(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
	{
		if(!CreateDirectory(strDirectory,   nullptr)   )
		{
			return FALSE;
		}
	} 
	return TRUE;
}


//删除文件夹
//BOOL DeleteFolder(CString strFilePath)
//{
//	if (strFilePath.Right(1) == _T("\\"))
//	{
//		strFilePath.TrimRight(_T("\\"));
//	}
//	strFilePath += _T("\0");
//	SHFILEOPSTRUCT fos ;
//	ZeroMemory( &fos, sizeof( fos)) ;
//	fos.hwnd   = HWND_DESKTOP;
//	fos.wFunc  = FO_DELETE ;
//	fos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION ;
//	fos.pFrom  = strFilePath ;
//	// 删除文件夹及其内容
//	if( 0 != SHFileOperation( &fos))
//		return FALSE ;
//	return TRUE;
//}

BOOL CPathStringFunction::DeleteDirectory(CString strDir)
{
	if(strDir.IsEmpty())   
	{ 
		return FALSE;
	} 
	
	if (strDir.Right(1) == _T("\\"))
	{
		strDir.TrimRight(_T("\\"));
	}

	//   首先删除文件及子文件夹 
	CFileFind   ff; 
	BOOL   bFound  =  ff.FindFile(strDir+ _T("\\*"), 0); 
	while(bFound) 
	{ 
		bFound = ff.FindNextFile(); 
		if(ff.GetFileName()== _T(".")||ff.GetFileName()== _T("..")) 
			continue; 

		//去掉文件(夹)只读等属性 
		SetFileAttributes(ff.GetFilePath(),   FILE_ATTRIBUTE_NORMAL); 
		if(ff.IsDirectory()) 
		{   
			DeleteDirectory(ff.GetFilePath()); 
			RemoveDirectory(ff.GetFilePath()); 
		} 
		else
		{ 
			DeleteFile(ff.GetFilePath());   //   删除文件 
		} 
	} 
	ff.Close(); 
	return RemoveDirectory(strDir); 
}
