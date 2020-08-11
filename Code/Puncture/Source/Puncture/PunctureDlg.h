//*****************************************************************
// 文件名 :						PunctureDlg..h
// 版本	 :						1.0
// 目的及主要功能 :				对话框应用程序的主头文件
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
#include "NDIOperator/NDIOperator.h"
#include "SurgicalPlan/SurgicalPlan.h"
//#include "SurgicalPlan/ImageSampler.h"
#include "USBCapturer/USBCapturer.h"
//#include "AnalyseProcess/FrameData.h"
#include "AnalyseProcess/AnalyseProcess.h"
//#include "FrameData.h"	
#include "CUSImageWnd.h"
#include "SceneWnd.h"

using namespace NDIOPERATOR;
using namespace SURGICALPLAN;
using namespace USBCAPTURER;
using namespace ANALYSEPROCESS;


/*****************************************************************
Defines
*****************************************************************/


/*****************************************************************
						Routine Definitions
*****************************************************************/
// CPunctureDlg 对话框
class CPunctureDlg : public CDialogEx
{
// 构造
public:
	CPunctureDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUNCTURE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	virtual BOOL DestroyWindow();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	int Init();								//初始化设备
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnBnClickedInportData();				//导入数据响应函数
	afx_msg void OnBnClickedLockCoordinate();			//关联响应函数
	afx_msg void OnBnClickedQuit();

public:
	CUSImageWndPtr	m_p2DShowWnd;		//2D显示的窗口类

	BOOL			m_b2Dshow;			//图像是否过来标志位
	BOOL			m_b3Dshow;			//3DMRI是否可以显示

	BOOL			m_bLock;			//是否关联的标志位

	CMutex			m_ShowMutex;		//显示互斥变量
	CMutex			m_PosMutex;			//NDI取用互斥变量

private:
	//function
	afx_msg void OnSize(UINT nType, int cx, int cy);	//重载调整大小
	void ArrangeCtrl();									//重新根据实际窗口尺寸调整控件大小和位置

	afx_msg void OnTimer(UINT_PTR nIDEvent);			//定时器，用于定时刷新

	void UpdateFrame(FrameDataPtr t_FrameDataPtr);		//传递给AnalyzeProcess，作为回调函数

	enum { IMAGE_UPDATER_TIMER = 2000 };
	double Random(double start, double end);

private:
	//var
	SceneWndPtr			m_p3DShowWnd;			//3D显示的窗口类

	NDIOperatorPtr		m_NDIOperatorPtr;		//NDI设备操作指针
	USBCapturerPtr		m_USBCapturerPtr;		//B超图像采集设备指针
	SurgicalPlanPtr		m_SurgicalPlanPtr;		//手术计划模块指针

	//AnalyzeProcessPtr	m_AnalyzeProcessPtr;	//分析模块指针
	AnalyseProcessPtr	m_AnalyseProcessPtr;	//分析模块指针


	FrameDataPtr		m_FrameDataPtr;			//单帧数据
	
	//多线程
	HANDLE				m_hInitThread;			//初始化线程
};
