#pragma once
namespace fsutility
{
class AFX_EXT_CLASS PathUtility
{
public:
	PathUtility(void);
	virtual ~PathUtility(void);

	//判断路径是否存在
	static BOOL DirExists(LPCTSTR   lpszDir);

	//递归删除目录
	static BOOL DeleteFolder( LPCTSTR pszFolder);

	//获取当前目录的父目录
	static CString GetParentPath(const CString &strPath);


	//获取文件所在目录
	static CString ExtractFilePath(const CString& strPath);

	/**
	* Find all sub-directory of @strPath. 
	* @return returns a list of sub dir name.
	*/
	static void FindAllSubDir(const CString& strAbsolutePath, 
		CList<CString>& listDir, BOOL bIncludeParentPath = TRUE);
	/**
	* Find all files in directory, exclude sub-dir
	* @param bIncludePathName if TRUE, the result in @listFileName will be
	*  absolute file name; or without path name
	*/
	static void FindAllFileinPath(const CString& strAbsolutePath, 
		CList<CString>& listFileName, BOOL bIncludePathName = TRUE);

	static BOOL IsFileinPath(const CString& strFile2Find,
		const CString& strPath);
	/**
	* @Remove specified directory and all it's files
	*/
	static BOOL RecurRMDir(const CString& strDir);

	static BOOL IsDir(const CString& strPath);

	/**
	* @Determines whether a path to a file system object such as a file or directory is valid.
	*/
	static BOOL IsPathFileExists(const CString& strPathorFile);

	static BOOL RecurMakeDir(const CString& strFullPath);

	/**
	* @Description: Recursively copy all contents of @strFromDir into @strToDir
	*/
	static BOOL RecurCopyDir(const CString& strFromDir, const CString& strToDir);

};
}
