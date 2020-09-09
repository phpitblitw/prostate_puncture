//*****************************************************************
// 文件名 :						Puncture.cpp
// 版本	 :						1.0
// 目的及主要功能 :				定义应用程序的类行为
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "stdafx.h"
#include "Puncture.h"
#include "PunctureDlg.h"

#include "ComUtility/SysPathManager.h"

using namespace fsutility;

///////////////////////////////////////////////////////
////////////////////////define/////////////////////////
///////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPunctureApp

BEGIN_MESSAGE_MAP(CPunctureApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPunctureApp 构造

CPunctureApp::CPunctureApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CPunctureApp 对象

CPunctureApp theApp;


 
/*****************************************************************
Name:			InitInstance
Inputs:
	none
Return Value:
	none
Description:	CPunctureApp 初始化
*****************************************************************/
BOOL CPunctureApp::InitInstance()
{
	//如果有一个版本运行就关闭
	if (IsProgrameRunning())
	{
		return FALSE;
	}

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);


	////////////////////////////////////////////系统初始化工作begin///////////////////////////////////////////////
	//Gdiplus
	Gdiplus::GdiplusStartupInput StartupInput;
	GdiplusStartup(&m_gdiplusToken, &StartupInput, nullptr);

	//加载日志
	SetDefaultFileAppender();
	////////////////////////////////////////////系统初始化工作end///////////////////////////////////////////////


	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CPunctureDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



/*****************************************************************
Name:			ExitInstance
Inputs:
	none
Return Value:
	int
Description:	CAutoScanerApp 退出函数
*****************************************************************/
int CPunctureApp::ExitInstance()
{
	LIST_DEBUG(_T("主模块关闭"));

	// TODO: 在此添加专用代码和/或调用基类
	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	ReleaseSemaphore(m_hSem, 1, nullptr);

	CloseHandle(m_hSem);

	//单线程无需关闭
	//CefShutdown();

	LIST_DEBUG(_T("RelasePrinterModule"));
	
	return CWinApp::ExitInstance();
}//ExitInstance


/*****************************************************************
Name:			SetDefaultFileAppender
Inputs:
	none
Return Value:
	none
Description:	设置日志文件路径
*****************************************************************/
void CPunctureApp::SetDefaultFileAppender()
{
	SetCurrentDirectory(CSysPathManager::Instance().GetExePath());
	std::locale::global(std::locale("chs"));
	CString strLogConfigPath;
	strLogConfigPath = CSysPathManager::Instance().GetConfigPath() + _T("ConfigLog_Main.txt");	//日志文件路径
	PropertyConfigurator::doConfigure(strLogConfigPath.GetBuffer(0));
	//FY_DEBUG(_T("start Main Module......"));
	LIST_DEBUG(_T("主模块启动......"));
}//SetDefaultFileAppender


/*****************************************************************
Name:			IsProgrameRunning
Inputs:
	none
Return Value:
	BOOL - 是否运行
Description:	判断程序是否运行
*****************************************************************/
BOOL CPunctureApp::IsProgrameRunning(void)
{
	/////////////////////////////////////////////////////////////
	m_hSem = CreateSemaphore(nullptr, 1, 1, GetExeName());
	// 信号量已存在 
	// 信号量存在，则程序已有一个实例运行 
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// 关闭信号量句柄 
		CloseHandle(m_hSem);
		m_hSem = nullptr;

		// 寻找先前实例的主窗口 
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
		while (::IsWindow(hWndPrevious))
		{
			// 检查窗口是否有预设的标记? 
			// 有，则是我们寻找的主窗 
			if (::GetProp(hWndPrevious, GetExeName()))
			{
				//通知窗口执行多次创建的操作
				//SendMessage(hWndPrevious, WM_RESTOREMAINWND_MESSAGE, 0, 0);

				//// 主窗口已最小化，则恢复其大小 
				if (::IsIconic(hWndPrevious))
					::ShowWindow(hWndPrevious, SW_RESTORE);

				//// 将主窗激活 
				::SetForegroundWindow(hWndPrevious);

				//// 将主窗的对话框激活 
				::SetForegroundWindow(
					::GetLastActivePopup(hWndPrevious));
				// 退出本实例 
				return TRUE;
			}
			// 继续寻找下一个窗口 
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
		}
		// 前一实例已存在，但找不到其主窗 
		// 可能出错了 
		// 退出本实例 
		return TRUE;
	}

	return FALSE;
}//IsProgrameRunning


/*****************************************************************
Name:			GetExeName
Inputs:
	none
Return Value:
	CString - exe名称
Description:	获取exe名称
*****************************************************************/
CString CPunctureApp::GetExeName()
{
	if (m_pszAppName == nullptr)
	{
		return _T("123456789");
	}
	CString strTemp = m_pszAppName;
	return (strTemp + _T("123456789"));
}//GetExeName


/*****************************************************************
Name:			RegiserMainWndProp
Inputs:
	HWND hMainWnd
Return Value:
	none
Description:	Main Frame OnCreate 调用
*****************************************************************/
void CPunctureApp::RegiserMainWndProp(HWND hMainWnd)
{
	::SetProp(hMainWnd, GetExeName(), (HANDLE)1);  //设置使得程序只运行一个实例
}//RegiserMainWndProp


/*****************************************************************
Name:			RegiserMainWndProp
Inputs:
	HWND hMainWnd
Return Value:
	none
Description:	Main Frame OnDestroy 调用
*****************************************************************/
void CPunctureApp::UnRegiserMainWndProp(HWND hMainWnd)
{
	::RemoveProp(hMainWnd, GetExeName());
}//UnRegiserMainWndProp