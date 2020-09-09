//*****************************************************************
// 文件名 :						Puncture..h
// 版本	 :						1.0
// 目的及主要功能 :				PROJECT_NAME 应用程序的主头文件
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					
//*****************************************************************/
#pragma once

/*****************************************************************
Library Files Included
*****************************************************************/
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


/*****************************************************************
Defines
*****************************************************************/

// CPunctureApp:
// 有关此类的实现，请参阅 Puncture.cpp
//

/*****************************************************************
						Routine Definitions
*****************************************************************/
class CPunctureApp : public CWinApp
{
public:
	CPunctureApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

	virtual int ExitInstance();

	//用于避免重复打开
	CString GetExeName();		//获取exe名称	
	BOOL IsProgrameRunning(void);	//判断程序是否运行，
	virtual void RegiserMainWndProp(HWND hMainWnd);
	virtual void UnRegiserMainWndProp(HWND hMainWnd);

	//用于日志文件
	void SetDefaultFileAppender();	//设置日志文件路径

public:
	HANDLE  m_hSem;
	ULONG_PTR m_gdiplusToken;
};

extern CPunctureApp theApp;
