//*****************************************************************
// 文件名 :						Calibrate.cpp
// 版本	 :						1.0
// 目的及主要功能 :				标定辅助软件
// 创建日期 :					2020.05.19
// 修改日期 :					
// 作者 :						孙易辙
// 修改者 :						
// 联系方式 :					
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "pch.h"
#include "framework.h"
#include "Calibrate.h"
#include "CalibrateDlg.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"	//待删除
#include "ErrorManager//ErrorCodeDefine.h"
#include "ComUtility/SysPathManager.h"

using namespace fsutility;
//TODO

///////////////////////////////////////////////////////
////////////////////////define/////////////////////////
///////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*****************************************************************
Global Variables
*****************************************************************/

/*****************************************************************
Function
*****************************************************************/

/*****************************************************************
Name:			InitThread
Inputs:
	LPVOID lpParam - 指针
Return Value:
	DWORD 1
Description:	初始化线程
*****************************************************************/
DWORD WINAPI InitThread(LPVOID lpParam)			//
{
	//设置按钮状态
	CCalibrateDlg *t_pPoint;
	t_pPoint = (CCalibrateDlg *)lpParam;

	t_pPoint->Init();		//初始化设备

	return 1;
}//InitThread

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCalibrateDlg 对话框



CCalibrateDlg::CCalibrateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALIBRATE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalibrateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCalibrateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CAPTURE, &CCalibrateDlg::OnBnClickedBtnCapture)
END_MESSAGE_MAP()


// CCalibrateDlg 消息处理程序

BOOL CCalibrateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hInitThread = CreateThread(nullptr, 0, InitThread, (void *)this, 0, nullptr);	//启动初始化线程

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCalibrateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCalibrateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCalibrateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CCalibrateDlg::Init()
{
	
	Invalidate(false);
	//窗口初始化
	CRect rect;
	GetDlgItem(IDC_USIMG)->GetWindowRect(rect);
	GetDlgItem(IDC_USIMG)->ShowWindow(SW_HIDE);
	ScreenToClient(rect);
	m_pUSShowWnd->MoveWindow(rect);

	//cv::Mat dog = cv::imread("D:\\prostate\\dog.jpg");
	//m_pUSShowWnd->UpdateSrcImage(dog);
	cv::Mat initialImg(300, 300, CV_8UC3);
	m_pUSShowWnd->UpdateSrcImage(initialImg);
	m_pUSShowWnd->RedrawWindow();

	CString t_strIniFileNamePath;

	//启动ndi设备
	t_strIniFileNamePath = CSysPathManager::Instance().GetConfigPath();
	t_strIniFileNamePath = t_strIniFileNamePath + _T("NDIDevice.ini");

	m_NDIOperatorPtr.reset(new NDIOperator());
	m_NDIOperatorPtr->BindUpdateAttitudeEvent(std::bind(&CCalibrateDlg::UpdateNDIData, this, std::placeholders::_1));
	if (m_NDIOperatorPtr->InitNDIDevice(t_strIniFileNamePath) != LIST_NO_ERROR)
	{
		MessageBox(_T("启动NDI设备失败"));
		return ER_InitNDIDeviceFailed;
	}
	m_NDIOperatorPtr->StartTracking();

	//启动超声设备
	t_strIniFileNamePath = CSysPathManager::Instance().GetConfigPath();
	t_strIniFileNamePath = t_strIniFileNamePath + _T("USBDevice.ini");

	m_USBCapturerPtr.reset(new USBCapturer());
	m_USBCapturerPtr->BindCapturePerFrameEvent(std::bind(&CCalibrateDlg::UpdateUSData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	if (m_USBCapturerPtr->InitUSBDevice(t_strIniFileNamePath) != LIST_NO_ERROR)
	{
		MessageBox(_T("启动B超图像采集设备失败"));
		return ER_InitUSBDeviceFailed;
	}
	m_USBCapturerPtr->StartGrab();
	
	//设置定时器 实时刷新参数
	SetTimer(US_UPDATER_TIMER, 1000, nullptr);

	Invalidate(false);
	return 0;
}

int CCalibrateDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pUSShowWnd.reset(new CUSImgWnd());
	m_pUSShowWnd->CreateWnd(this, CRect(0, 0, 10, 10), WS_VISIBLE | WS_CHILD | WS_BORDER, NULL, NULL, 103);
	return 0;
}

void CCalibrateDlg::OnTimer(UINT_PTR nIDEvent)
{
	CSingleLock t_ShowLocker(&m_ShowMutex);
	CSingleLock t_PosLocker(&m_PosMutex);

	//2D显示
	t_ShowLocker.Lock();
	if (m_pUSShowWnd != NULL)
	{
		m_pUSShowWnd->UpdateSrcImage(m_imgUS);
		m_pUSShowWnd->RedrawWindow();
	}
	t_ShowLocker.Unlock();
	//更新NDI位置
	CString str;
	//str.Format(_T("%f"),m_attitude.GetPosition)
	float fX, fY, fZ, fAlpha, fBeta, fGama;
	t_PosLocker.Lock();
	m_attitude.GetPosition(fX, fY, fZ, fAlpha, fBeta, fGama);
	t_PosLocker.Unlock();
	str.Format(_T("%f"), fX);
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	str.Format(_T("%f"), fY);
	GetDlgItem(IDC_EDIT2)->SetWindowText(str);
	str.Format(_T("%f"), fZ);
	GetDlgItem(IDC_EDIT3)->SetWindowText(str);
	str.Format(_T("%f"), fAlpha);
	GetDlgItem(IDC_EDIT4)->SetWindowText(str);
	str.Format(_T("%f"), fBeta);
	GetDlgItem(IDC_EDIT5)->SetWindowText(str);
	str.Format(_T("%f"), fGama);
	GetDlgItem(IDC_EDIT6)->SetWindowText(str);
	return;
}

void CCalibrateDlg::UpdateNDIData(std::vector<NDIOPERATOR::Attitude> t_Attitude)
{
	CSingleLock singlelock(&m_PosMutex);
	singlelock.Lock();
	m_attitude = t_Attitude[0];
	m_attitudes = t_Attitude;
	singlelock.Unlock();
}

void CCalibrateDlg::UpdateUSData(cv::Mat t_USBImgA, cv::Mat t_USBImgB, double t_dImageRes)
{
	CSingleLock singlelock(&m_ShowMutex);
	singlelock.Lock();
	m_imgUS = t_USBImgA;
	m_dImageRes = t_dImageRes;
	singlelock.Unlock();
}

/*
int counter = 0;	//采集标定数据计数器 TODO
void CCalibrateDlg::OnBnClickedBtnCapture()
{
	// TODO: 在此添加控件通知处理程序代码
	CString t_strFileNamePath,t_strImgNamePath;
	CString t_strInfo;
	float fX, fY, fZ, fAlpha, fBeta, fGama;
	m_attitude.GetPosition(fX, fY, fZ, fAlpha, fBeta, fGama);

	//存储ndi设备信息
	t_strFileNamePath = CSysPathManager::Instance().GetConfigPath();
	//t_strFileNamePath = t_strFileNamePath + _T("BaseParameters.txt"); //TODO

	//50组采集 TODO
	CString t_strSuffix;
	counter++;
	if (counter == 120)	//控制自动采集120组
	{
		MessageBox(_T("采集结束"));
		return;
	}
	t_strSuffix.Format(_T("_%d"), counter);
	t_strFileNamePath = t_strFileNamePath +(_T("标定数据采集//")) +_T("BaseParameters")+ t_strSuffix+_T(".txt");

	CFile fileOut;
	if (!fileOut.Open(t_strFileNamePath, CFile::modeWrite | CFile::modeCreate))
	{
		MessageBox(_T("存储路径打开失败"));
		return;	//无法生成文件错误
	}
	t_strInfo.Format(_T("fX\t%f\n"), fX);
	fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	t_strInfo.Format(_T("fY\t%f\n"), fY);
	fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	t_strInfo.Format(_T("fZ\t%f\n"), fZ);
	fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	t_strInfo.Format(_T("fAlpha\t%f\n"), fAlpha);
	fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	t_strInfo.Format(_T("fBeta\t%f\n"), fBeta);
	fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	t_strInfo.Format(_T("fGama\t%f\n"), fGama);
	fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	
	//存储超声信息
	t_strInfo.Format(_T("USImageRes\t%f\n"), m_dImageRes);	//超声图像 像素物理尺寸(mm)
	fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	fileOut.Close();
	USES_CONVERSION;
	//t_strImgNamePath = CSysPathManager::Instance().GetConfigPath() + _T("baseUS.bmp"); //TODO
	t_strImgNamePath= CSysPathManager::Instance().GetConfigPath() + (_T("50组标定数据//")) + _T("baseUS") + t_strSuffix + _T(".bmp");
	cv::String fPath = W2A(t_strImgNamePath);
	cv::imwrite(fPath, m_imgUS);
	//cv::imwrite()
}
*/


//存储单帧B超&NDI数据 以counter序号存储
void CCalibrateDlg::SaveOneFrame(int counter)
{
	CString t_strFileNamePath, t_strImgNamePath;
	CString t_strInfo;
	int i;
	float fX, fY, fZ, fAlpha, fBeta, fGama;
	

	//存储NDI设备信息
	CString t_strSuffix;
	t_strSuffix.Format(_T("_%d"), counter);
	t_strFileNamePath = CSysPathManager::Instance().GetConfigPath() + (_T("标定数据采集//")) + _T("BaseParameters") + t_strSuffix + _T(".txt");

	CFile fileOut;
	if (!fileOut.Open(t_strFileNamePath, CFile::modeWrite | CFile::modeCreate))
	{
		MessageBox(_T("存储路径打开失败"));
		return;	//无法生成文件错误
	}
	for (i = 0; i < m_attitudes.size(); i++)
	{
		m_attitudes[i].GetPosition(fX, fY, fZ, fAlpha, fBeta, fGama);
		//存储NDI设备信息
		t_strInfo.Format(_T("fX\t%f\n"), fX);
		fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
		t_strInfo.Format(_T("fY\t%f\n"), fY);
		fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
		t_strInfo.Format(_T("fZ\t%f\n"), fZ);
		fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
		t_strInfo.Format(_T("fAlpha\t%f\n"), fAlpha);
		fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
		t_strInfo.Format(_T("fBeta\t%f\n"), fBeta);
		fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
		t_strInfo.Format(_T("fGama\t%f\n"), fGama);
		fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	}
	
	//存储超声信息
	t_strInfo.Format(_T("USImageRes\t%f\n"), m_dImageRes);	//超声图像 像素物理尺寸(mm)
	fileOut.Write(t_strInfo.GetBuffer(), t_strInfo.GetLength() * sizeof(wchar_t));
	fileOut.Close();
	USES_CONVERSION;
	//t_strImgNamePath = CSysPathManager::Instance().GetConfigPath() + _T("baseUS.bmp"); //TODO
	t_strImgNamePath = CSysPathManager::Instance().GetConfigPath() + (_T("标定数据采集//")) + _T("baseUS") + t_strSuffix + _T(".bmp");
	cv::String fPath = W2A(t_strImgNamePath);
	cv::imwrite(fPath, m_imgUS);
}

void CCalibrateDlg::OnBnClickedBtnCapture()
{
	int counter = 0;
	while (counter < 360)
	{
		SaveOneFrame(counter);
		Sleep(300);
		counter++;
	}
	MessageBox(_T("采集结束"));
	return;
}