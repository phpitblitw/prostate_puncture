#include "StdAfx.h"
#include "DiskManager.h"

using namespace fsutility;

CDiskManager::CDiskManager(void)
{
}

CDiskManager::~CDiskManager(void)
{
}

float CDiskManager::GetDiscSpaceFree(CString path)
{
	float   ffreesize;
	CString dirname = _T("");
	int index = path.Find('\\');
	dirname = path.Left(index+1);

	UINT64 i64FreeBytesAvailable, i64TotalNumberOfBytes;
	GetDiskFreeSpaceEx(dirname, 
		(PULARGE_INTEGER)&i64FreeBytesAvailable, 
		(PULARGE_INTEGER)&i64TotalNumberOfBytes, 
		nullptr);
	ffreesize =(float)(i64FreeBytesAvailable/(1024.0 * 1024.0));
	return ffreesize;
}