//*****************************************************************
// 文件名 :						PunctureDlg.cpp
// 版本	 :						1.0
// 目的及主要功能 :				对话框实现文件
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
#include "afxdialogex.h"
#include "ErrorManager//ErrorCodeDefine.h"
#include "ComUtility/SysPathManager.h"

using namespace fsutility;

///////////////////////////////////////////////////////
////////////////////////define/////////////////////////
///////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*****************************************************************
Global Variables
*****************************************************************/
CPunctureDlg *pPuncturedlg;

/*****************************************************************
Function
*****************************************************************/
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
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
	CPunctureDlg *t_pPoint;
	t_pPoint = (CPunctureDlg *)lpParam;

	t_pPoint->Init();		//初始化设备

	return 1;
}//InitThread



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


/////////////////// CPunctureDlg 对话框/////////////////////////////
/*****************************************************************
Name:			CPunctureDlg
Inputs:
	none
Return Value:
	none
Description:	CPunctureDlg构造函数
*****************************************************************/
CPunctureDlg::CPunctureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PUNCTURE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//显示窗口类初始化为空
	m_p2DShowWnd = nullptr;
	m_p3DShowWnd = nullptr;

	m_b2Dshow = false;
	m_b3Dshow = false;

	m_bLock = false;

	//对象初始化为空指针
	m_NDIOperatorPtr = nullptr;
	m_USBCapturerPtr = nullptr;

	//m_AnalyzeProcessPtr = nullptr;
	m_AnalyseProcessPtr = nullptr;

	m_FrameDataPtr = nullptr;	
}//CPunctureDlg


/*****************************************************************
Name:			DoDataExchange
Inputs:
	none
Return Value:
	none
Description:	数据交换
*****************************************************************/
void CPunctureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


/*****************************************************************
Name:			MESSAGE_MAP
Inputs:
	none
Return Value:
	none
Description:	消息映射
*****************************************************************/
BEGIN_MESSAGE_MAP(CPunctureDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_INPORTDATA, &CPunctureDlg::OnBnClickedInportData)
	ON_BN_CLICKED(IDC_LOCKCOORDINATE, &CPunctureDlg::OnBnClickedLockCoordinate)
	ON_BN_CLICKED(IDC_QUIT, &CPunctureDlg::OnBnClickedQuit)
END_MESSAGE_MAP()


// CPunctureDlg 消息处理程序
/*****************************************************************
Name:			OnInitDialog
Inputs:
	none
Return Value:
	true  -	成功初始化
	false -	初始化失败
Description:	初始化函数
*****************************************************************/
BOOL CPunctureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//全屏显示
	int cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);

	CRect rcTemp(0, 0, cx, cy);
	MoveWindow(&rcTemp);

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
	//对显示部分窗口类进行初始化

	// TODO: 在此添加额外的初始化代码
	ArrangeCtrl();					//排列对话框布局
	
	pPuncturedlg = this;

	m_hInitThread = CreateThread(nullptr, 0, InitThread, (void *)this, 0, nullptr);	//启动刷新显示线程

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}//OnInitDialog


/*****************************************************************
Name:			OnSysCommand
Inputs:
	none
Return Value:
	none
Description:	系统函数
*****************************************************************/
void CPunctureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
}//OnSysCommand


/*****************************************************************
Name:			OnPaint
Inputs:
	none
Return Value:
	none
Description:	绘制函数
				如果向对话框添加最小化按钮，则需要下面的代码
				来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
				这将由框架自动完成。
*****************************************************************/
void CPunctureDlg::OnPaint()
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
}//OnPaint


/*****************************************************************
Name:			OnQueryDragIcon
Inputs:
	none
Return Value:
	none
Description:	当用户拖动最小化窗口时系统调用
				此函数取得光标显示。
*****************************************************************/
HCURSOR CPunctureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}//OnQueryDragIcon


/*****************************************************************
Name:			OnCreate
Inputs:
	none
Return Value:
	none
Description:	创建各显示窗口，初始化
				2D和3D的窗口
*****************************************************************/
int CPunctureDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_p2DShowWnd.reset(new CUSImageWnd);
	m_p2DShowWnd->CreateWnd(this, CRect(0, 0, 10, 10), WS_VISIBLE | WS_CHILD | WS_BORDER, NULL, NULL, 101);

	m_p3DShowWnd.reset(new SceneWnd);
	m_p3DShowWnd->CreateWnd(this, CRect(0, 0, 10, 10), WS_VISIBLE | WS_CHILD | WS_BORDER, NULL, NULL, 102);

	return 0;
}//OnCreate


/*****************************************************************
Name:			DestroyWindow
Inputs:
	none
Return Value:
	none
Description:	销毁窗口时，销毁各显示窗口
*****************************************************************/
BOOL CPunctureDlg::DestroyWindow()
{
	//停止分析
	if (m_AnalyseProcessPtr != nullptr)
	{
		m_AnalyseProcessPtr->StopAnalyse();
	}

	//销毁窗口
	if (m_p2DShowWnd != nullptr)
	{
		m_p2DShowWnd->DestroyWindow();
	}

	if (m_p3DShowWnd != nullptr)
	{
		m_p3DShowWnd->DestroyWindow();
	}

	//停止采集
	if (m_NDIOperatorPtr != nullptr)
	{
		m_NDIOperatorPtr->StopTracking();
	}
	
	if (m_USBCapturerPtr != nullptr)
	{
		m_USBCapturerPtr->StopGrab();
	}
	
	return CDialogEx::DestroyWindow();
}//DestroyWindow


/***********************************自建函数***************************************/
/*****************************************************************
Name:			Init(待完成)
Inputs:
	none
Return Value:
	int - Error Info
Description:	初始化整个系统，
				1.读入config文件
				2.创建设备指针
				3.初始化各设备
				4.创建处理模块
				4.初始化处理模块
				5.设置初始状态
				6.刷新控件显示
*****************************************************************/
int CPunctureDlg::Init()
{	
	//灰化按钮
	GetDlgItem(IDC_INPORTDATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOCKCOORDINATE)->EnableWindow(FALSE);

	Invalidate(false);

	//创建设备指针
	CString t_strIniFileNamePath;
	//初始化NDI设备
	t_strIniFileNamePath = CSysPathManager::Instance().GetConfigPath();
	t_strIniFileNamePath = t_strIniFileNamePath + _T("NDIDevice.ini");

	m_NDIOperatorPtr.reset(new NDIOperator());
	if (m_NDIOperatorPtr->InitNDIDevice(t_strIniFileNamePath) != LIST_NO_ERROR)
	{
		MessageBox(_T("启动NDI设备失败"));
		return ER_InitNDIDeviceFailed;
	}

	//初始化USB视频采集卡
	t_strIniFileNamePath = CSysPathManager::Instance().GetConfigPath();
	t_strIniFileNamePath = t_strIniFileNamePath + _T("USBDevice.ini");

	m_USBCapturerPtr.reset(new USBCapturer());
	if (m_USBCapturerPtr->InitUSBDevice(t_strIniFileNamePath) != LIST_NO_ERROR)
	{
		MessageBox(_T("启动B超图像采集设备失败"));
		return ER_InitUSBDeviceFailed;
	}
	
	//创建手术计划模块
	t_strIniFileNamePath = CSysPathManager::Instance().GetConfigPath();
	t_strIniFileNamePath = t_strIniFileNamePath + _T("SurgicalPlan.ini");
	m_SurgicalPlanPtr.reset(new SurgicalPlan());
	if (m_SurgicalPlanPtr->InPortAsFileSet(t_strIniFileNamePath) != LIST_NO_ERROR)
	{
		MessageBox(_T("创建手术计划模块失败"));
		return ER_FileOpenFailed;
	}

	//创建分析处理模块
	t_strIniFileNamePath = CSysPathManager::Instance().GetConfigPath();
	t_strIniFileNamePath = t_strIniFileNamePath + _T("AnalyseProcess.ini");

	m_AnalyseProcessPtr.reset(new AnalyseProcess());
	m_AnalyseProcessPtr->BindUpdateFrameEvent(std::bind(&CPunctureDlg::UpdateFrame, this, std::placeholders::_1));
	m_AnalyseProcessPtr->SetNDIDevicePtr(m_NDIOperatorPtr);
	m_AnalyseProcessPtr->SetUSBCapturerPtr(m_USBCapturerPtr);
	m_AnalyseProcessPtr->SetSurgicalPlanPtr(m_SurgicalPlanPtr);

	if (m_AnalyseProcessPtr->InitAnalyseProcess(t_strIniFileNamePath) != LIST_NO_ERROR)
	{
		MessageBox(_T("启动分析模块失败"));
		return ER_InitAnalyseProcessFailed;
	}

	//设置显示模块尺寸(逻辑像素数  同截取的B超图像尺寸)
	int t_nShowImageX, t_nShowImageY;
	//m_p2DShowWnd->GetImageSize(t_nShowImageX, t_nShowImageY);
	m_USBCapturerPtr->GetImageSize(t_nShowImageX,t_nShowImageY);
	m_AnalyseProcessPtr->Set2DImageSize(t_nShowImageX, t_nShowImageY);


	////设置显示模块的尺寸
	//int t_nShowImageX, t_nShowImageY;
	//m_p2DShowWnd->GetImageSize(t_nShowImageX, t_nShowImageY);
	//t_nShowImageX = 456, t_nShowImageY = 363;//未读取到正确的2d窗口尺寸
	//m_AnalyzeProcessPtr.reset(new AnalyzeProcess());
	//m_AnalyzeProcessPtr->Set2DImageSize(t_nShowImageX, t_nShowImageY);

	
	float xmax = 200, ymax = 200, zmax = 88.55f;
	m_p3DShowWnd->AttachSurfaceData(m_SurgicalPlanPtr->GetSurfaceDataPtr());
	m_p3DShowWnd->SetBoundBox(0, xmax, 0, ymax, 0, zmax);	//此处SurfaceData并未初始化 TODO

	////找到获取前列腺最大切面
	//m_SurgicalPlanPtr.reset(new SurgicalPlan());
	//m_SurgicalPlanPtr->SetImageSize(t_nShowImageX, t_nShowImageY);
	//m_SurgicalPlanPtr->FindLargestSection();

	//设置硬件及数据指针
	//m_AnalyseProcessPtr->SetNDIDevicePtr(m_NDIOperatorPtr);
	//m_AnalyseProcessPtr->SetUSBCapturerPtr(m_USBCapturerPtr); 
	//m_AnalyseProcessPtr->SetSurgicalPlanPtr(m_SurgicalPlanPtr);

	//启动分析
	m_NDIOperatorPtr->StartTracking();
	m_USBCapturerPtr->StartGrab();
	m_AnalyseProcessPtr->StartAnalyse();

	//每隔100ms刷新一次图像
	SetTimer(IMAGE_UPDATER_TIMER, 100, nullptr);

	//刷新控件显示
	GetDlgItem(IDC_INPORTDATA)->EnableWindow(TRUE);
	//GetDlgItem(IDC_LOCKCOORDINATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOCKCOORDINATE)->EnableWindow(TRUE);

	Invalidate(false);
	
	return LIST_NO_ERROR;
}//Init


/*****************************************************************
Name:			OnSize(待完成)
Inputs:
	none
Return Value:
	int - Error Info
Description:	//重载调整大小
*****************************************************************/
void CPunctureDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	ArrangeCtrl();		//重新布局各空间位置和大小
}//OnSize


/*****************************************************************
Name:			ArrangeCtrl
Inputs:
	none
Return Value:
	none
Description:	重新根据实际窗口尺寸
				调整控件大小和位置
*****************************************************************/
void CPunctureDlg::ArrangeCtrl()
{
	//全屏显示
	int cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);

	CRect rcTemp;
	rcTemp.BottomRight() = CPoint(cx, cy);
	rcTemp.TopLeft() = CPoint(0, 0);
	MoveWindow(&rcTemp);

	CRect viewRect, fullImageRect;
	GetClientRect(&viewRect);
	int nFullWidth;

	//设定显示窗口位置及大小
	nFullWidth  = (int)(cy - 20);
	if (GetDlgItem(IDC_USBIMG)->GetSafeHwnd())
	{
		fullImageRect.left = viewRect.left + 10;
		fullImageRect.top = viewRect.top + 5;
		fullImageRect.right = fullImageRect.left + nFullWidth;
		fullImageRect.bottom = viewRect.bottom - 30;
		GetDlgItem(IDC_USBIMG)->MoveWindow(fullImageRect);
		if (m_p2DShowWnd != NULL)
		{
			m_p2DShowWnd->MoveWindow(fullImageRect);				//这个m_p2DShowWnd没有初始化，而是null，我在构造函数里初始化了一下，智能指针用这个方法初始化
		}

		if (m_AnalyseProcessPtr != nullptr)
		{
			//设置显示模块的尺寸
			int t_nShowImageX, t_nShowImageY;
			m_p2DShowWnd->GetImageSize(t_nShowImageX, t_nShowImageY);
			m_AnalyseProcessPtr->Set2DImageSize(t_nShowImageX, t_nShowImageY);
		}
	}

	nFullWidth = (int)(cx - cy - 20);
	if (GetDlgItem(IDC_MRIIMG)->GetSafeHwnd())
	{
		fullImageRect.left = viewRect.left + cy;
		fullImageRect.top = viewRect.top + 5;
		fullImageRect.right = fullImageRect.left + nFullWidth;
		fullImageRect.bottom = fullImageRect.top + nFullWidth;
		GetDlgItem(IDC_MRIIMG)->MoveWindow(fullImageRect);
		if (m_p3DShowWnd != NULL)
		{
			m_p3DShowWnd->MoveWindow(fullImageRect);
		}

		if (m_AnalyseProcessPtr != nullptr)
		{
			//设置显示模块的尺寸
			int t_nShowImageX, t_nShowImageY;
			m_p2DShowWnd->GetImageSize(t_nShowImageX, t_nShowImageY);
			m_AnalyseProcessPtr->Set2DImageSize(t_nShowImageX, t_nShowImageY);
		}
	}

	//设定按钮位置及大小
	fullImageRect.left = viewRect.left + cy + ( cx - cy ) / 2 -275;
	fullImageRect.top = fullImageRect.top + nFullWidth + (cy - nFullWidth) / 2;		// -55;

	if (GetDlgItem(IDC_INPORTDATA)->GetSafeHwnd())
	{
		fullImageRect.right = fullImageRect.left + 150;
		fullImageRect.bottom = fullImageRect.top + 50;
		GetDlgItem(IDC_INPORTDATA)->MoveWindow(fullImageRect);
	}

	if (GetDlgItem(IDC_LOCKCOORDINATE)->GetSafeHwnd())
	{
		fullImageRect.left += 200;
		fullImageRect.right = fullImageRect.left + 150;
		GetDlgItem(IDC_LOCKCOORDINATE)->MoveWindow(fullImageRect);
	}

	if (GetDlgItem(IDC_QUIT)->GetSafeHwnd())
	{
		fullImageRect.left += 200;
		fullImageRect.right = fullImageRect.left + 150;
		GetDlgItem(IDC_QUIT)->MoveWindow(fullImageRect);
	}
}//ArrangeCtrl


/*****************************************************************
Name:			Random
Inputs:
	none
Return Value:
	none
Description:	生成double型随机数
*****************************************************************/
double CPunctureDlg::Random(double start, double end)
{
	double dis = end - start;
	return start + dis * (rand() / (RAND_MAX + 1.0));
}//Random


/*****************************************************************
Name:			OnTimer
Inputs:
	UINT_PTR nIDEvent - 定时类型
Return Value:
	none
Description:	定时刷新图像
*****************************************************************/
void CPunctureDlg::OnTimer(UINT_PTR nIDEvent)
{	
	//FY_DEBUG(_T("OnTime end"));
	CSingleLock t_ShowLocker(&m_ShowMutex);
	t_ShowLocker.Lock();

	//此处加入显示代码
	if (m_p2DShowWnd != NULL && m_p3DShowWnd != NULL)
	{
		//取数据区指针
		if (m_FrameDataPtr == nullptr)
		{
			return;
		}

		//2D显示窗口
		if (m_b2Dshow)
		{
			m_p2DShowWnd->UpdateSrcImage(m_FrameDataPtr->m_USBImage);
			m_p2DShowWnd->UpdateMaskImage(m_FrameDataPtr->m_pFusionMask);
			m_p2DShowWnd->RedrawWindow();
		}

		//3d显示窗口，等待FrameData位置参数完善后处理 TODO
		if (m_b3Dshow)
		{
			if (m_bLock)//关联之后的位姿
			{
				//更新探头顶点位置
				m_p3DShowWnd->UpdateScanCenter(m_FrameDataPtr->m_ScanCenter.x, m_FrameDataPtr->m_ScanCenter.y, m_FrameDataPtr->m_ScanCenter.z);
				//更新截面姿态参数 TODO
				//m_SurgicalPlanPtr->GetSurfaceDataPtr()->SetSamplePlan();
			}
			else//关联之前的位姿
			{
				m_p3DShowWnd->UpdateScanCenter(0, 0, 0);
			}
			m_p3DShowWnd->RedrawWindow();
		}
		/*
		if (m_b3Dshow)
		{
			double mindis = -2.0, maxdis = 2.0; //距离变化范围
			double minangle = -0.1, maxangle = 0.1; //法向量变化范围

			SURGICALPLAN::Point3D MoveDir, UpDir, ScanCenter;
			////初始值
			//MoveDir.x = 0;
			//MoveDir.y = 0;
			//MoveDir.z = 1; //初始值为手动计算的
			//UpDir.x = 0;
			//UpDir.y = -1;
			//UpDir.z = 0; //模拟超声平面的法向量，实验阶段初始值设为y轴方向，实际需要改的
			//ScanCenter.x = 56.25;
			//ScanCenter.y = 132.5;
			//ScanCenter.z = 42.35;

			if (m_bLock)//关联之后的位姿
			{
				float tx, ty, tz, tA, tB, tG;
				m_FrameDataPtr->m_USBPos.GetPosition(tx, ty, tz, tA, tB, tG);
				ScanCenter.x = tx;
				ScanCenter.y = ty;
				ScanCenter.z = tz;
				//先设置默认的值
				MoveDir.x = tA / (2.0*PI);
				MoveDir.y = tB / (2.0*PI);
				MoveDir.z = tG / (2.0*PI);
				UpDir.x = tA / (2.0*PI);
				UpDir.y = tB / (2.0*PI);
				UpDir.z = tG / (2.0*PI);
			}
			else//关联之前的位姿
			{
				m_AnalyzeProcessPtr->GetParameters(ScanCenter.x, ScanCenter.y, ScanCenter.z,
					MoveDir.x, MoveDir.y, MoveDir.z, UpDir.x, UpDir.y, UpDir.z);
			}
			m_p3DShowWnd->UpdateScanCenter(ScanCenter.x, ScanCenter.y, ScanCenter.z);

			////B超探头运动的方向（即前进at的方向）
			//double dx, dy, dz;
			////随机生成数据
			//dx = MoveDir.x + Random(minangle, maxangle);
			//dy = MoveDir.y + Random(minangle, maxangle);
			//dz = MoveDir.z + Random(minangle, maxangle);
			////测试
			//m_pMaskSampler->SetMoveDir(dx, dy, dz);

			////B超平面的法向量（即相机的up方向）
			//double nx, ny, nz;
			////随机生成数据
			//nx = UpDir.x + Random(minangle, maxangle);
			//ny = UpDir.y + Random(minangle, maxangle);
			//nz = UpDir.z + Random(minangle, maxangle);
			////测试
			//m_pMaskSampler->SetPlaneDir(nx, ny, nz);

			////B超探头在体积中的中心位置（即图像bottom边的中心点），单位为mm
			//double x, y, z;
			////随机生成数据
			//x = ScanCenter.x + Random(mindis, maxdis);
			//y = ScanCenter.y + Random(mindis, maxdis);
			//z = ScanCenter.z + Random(mindis, maxdis);
			////测试
			//m_pMaskSampler->SetCenterPt(x, y, z);


			//m_pMaskSampler->SetMoveDir(MoveDir.x, MoveDir.y, MoveDir.z);
			//m_pMaskSampler->SetPlaneDir(UpDir.x, UpDir.y, UpDir.z);
			//m_pMaskSampler->SetCenterPt(ScanCenter.x, ScanCenter.y, ScanCenter.z);

			//m_p3DShowWnd->UpdateScanCenter(x, y, z);
			
		}
		*/
		
	}

	t_ShowLocker.Unlock();

	Invalidate(false);

	CWnd::OnTimer(nIDEvent);
}//OnTimer


/*****************************************************************
Name:			UpdateFrame
Inputs:
	FrameDataPtr m_FrameDataPtr - 从处理模块传回来的数据
Return Value:
	none
Description:	定时刷新图像
*****************************************************************/
void CPunctureDlg::UpdateFrame(FrameDataPtr t_FrameDataPtr)
{
	CSingleLock singlelock(&m_ShowMutex);
	singlelock.Lock();

	m_FrameDataPtr = t_FrameDataPtr;
	m_b2Dshow = true;	//标注2D图像已经获取
	m_b3Dshow = true;

	singlelock.Unlock();
}//UpdateFrame


/*****************************************************************
Name:			OnBnClickedInportData
Inputs:
	none
Return Value:
	none
Description:	打开手术计划
*****************************************************************/
void CPunctureDlg::OnBnClickedInportData()
{
	//流程
	//读取窗口大小
	//设置ana对应大小
	//设置3D窗口大小
	//读取文件
	//绑定3D数据

	CFileDialog DatafileDlg(TRUE,
		NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	DatafileDlg.m_ofn.lpstrTitle = _T("Please choose ini file");

	//获取原始的MRI数据
	CString FileName;
	if (DatafileDlg.DoModal() == IDOK)
	{
		// 打开MRI数据，mask。交给SurigicalPlan来做 TODO
		/*
		//打开文件组，在analyze process中
		FileName = DatafileDlg.GetPathName();
		if (m_AnalyzeProcessPtr->InportData(FileName) != LIST_NO_ERROR)
		{
			AfxMessageBox(_T("导入手术计划失败"));
			return;
		}
		
		//给示意三维窗口赋值
		m_p3DShowWnd->AttachSurfaceData(m_AnalyzeProcessPtr->GetSurfaceDataPtr());
		*/
		//double x, y, z;
		//m_AnalyzeProcessPtr->GetCenterPt(x, y, z);
		//m_p3DShowWnd->UpdateScanCenter(x, y, z);
		//m_b3Dshow = true;
	

		//if (m_SurgicalPlanPtr->GetMaskDataPtr()->LoadRawMask(FileName, cx, cy, cz))
		//{
		//	m_SurgicalPlanPtr->GetMaskDataPtr()->SetMaskVolumeResolution(resx, resy, resz);

		//	int ShowImageX, ShowImageY;
		//	m_p2DShowWnd->GetImageSize(ShowImageX, ShowImageY);
		//	BYTE *pMaskData = new BYTE[ShowImageX*ShowImageY];
		//	m_pMaskSampler->SetDataPackage(m_SurgicalPlanPtr);
		//	m_pMaskSampler->GetSampleMaskPlan(pMaskData, ShowImageX, ShowImageY);
		//	
		//	m_p2DShowWnd->UpdateMaskImage(pMaskData);
		//	m_p2DShowWnd->RedrawWindow();
		//	delete[]pMaskData;

		//	//给示意三维窗口赋值
		//	m_p3DShowWnd->AttachSurfaceData(m_SurgicalPlanPtr->GetSurfaceDataPtr());

		//	float xmax = 200, ymax = 200, zmax = 88.55f;
		//	m_p3DShowWnd->SetBoundBox(0, xmax, 0, ymax, 0, zmax);

		//	double x, y, z;
		//	m_pMaskSampler->GetCenterPt(x, y, z);

		//	m_p3DShowWnd->UpdateScanCenter(x, y, z);
		//	m_b3Dshow = true;
		//}	
	}
	else
	{
		return;
	}

	//CFileDialog DatafileDlg1(TRUE,
	//	_T("*.bmp"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	//DatafileDlg1.m_ofn.lpstrTitle = _T("Please choose BMP file");

	//if (DatafileDlg1.DoModal() == IDOK)
	//{
	//	//m_p2DShowWnd->LoadSrcImage(DatafileDlg1.GetPathName());
	//	m_p2DShowWnd->RedrawWindow();
	//}
	
	GetDlgItem(IDC_LOCKCOORDINATE)->EnableWindow(TRUE);
}//OnBnClickedInportData


/*****************************************************************
Name:			OnBnClickedLockCoordinate
Inputs:
	none
Return Value:
	none
Description:	关联相应函数
*****************************************************************/
void CPunctureDlg::OnBnClickedLockCoordinate()
{
	//m_AnalyzeProcessPtr->CalibratePlanUSBCoordinate();		//计算超声探头与手术计划数据之间的转换矩阵
	m_AnalyseProcessPtr->Register();			//计算超声探头与MRI模拟采样之间的转换矩阵

	m_bLock = true;
}//OnBnClickedLockCoordinate


/*****************************************************************
Name:			OnBnClickedQuit(待完成)
Inputs:
	none
Return Value:
	none
Description:	点击退出按钮，如还在采集分析中
				则停止所有工作，析构所有已开辟
				空间，退出程序
*****************************************************************/
void CPunctureDlg::OnBnClickedQuit()
{
	// TODO: 在此添加控件通知处理程序代码
}//OnBnClickedQuit