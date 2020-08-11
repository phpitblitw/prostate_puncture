//*****************************************************************
// 文件名 :						Calibrate.h
// 版本	 :						1.0
// 目的及主要功能 :				标定辅助软件
// 创建日期 :					2020.05.19
// 修改日期 :					
// 作者 :						孙易辙
// 修改者 :						
// 联系方式 :					
//*****************************************************************/
#pragma once

/*****************************************************************
Library Files Included
*****************************************************************/
//TODO
#include "CUSImgWnd.h"
#include "NDIOperator/NDIOperator.h"
#include "USBCapturer/USBCapturer.h"

using namespace NDIOPERATOR;
using namespace USBCAPTURER;

/*****************************************************************
Defines
*****************************************************************/

/*****************************************************************
						Routine Definitions
*****************************************************************/


// CCalibrateDlg 对话框
class CCalibrateDlg : public CDialogEx
{
// 构造
public:
	CCalibrateDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIBRATE_DIALOG };
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
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	int Init();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedBtnCapture();

	void UpdateNDIData(std::vector<Attitude> t_Attitude);							//更新NDI数据，回调函数，由NDI模块调用
	void UpdateUSData(cv::Mat t_USBImgA, cv::Mat t_USBImgB, double t_dImageRes);	//更新B超数据，回调函数，由B超模块调用
	void SaveOneFrame(int counter);													//存储单帧B超&NDI数据

public:
	CUSImgWndPtr	m_pUSShowWnd;
	NDIOperatorPtr	m_NDIOperatorPtr;		//NDI设备操作指针
	USBCapturerPtr	m_USBCapturerPtr;		//B超图像采集设备指针

	CMutex			m_ShowMutex;			//显示互斥变量
	CMutex			m_PosMutex;				//NDI取用互斥变量

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);		//定时器，用于定时刷新
	enum { US_UPDATER_TIMER = 3000 };

private:
	HANDLE	m_hInitThread;		//初始化线程
	//CMutex	m_UpdateDataMutex;	//更新参数互斥变量
	Attitude m_attitude;		//NDI位置
	vector<Attitude> m_attitudes;	//NDI位置(多探头)
	cv::Mat m_imgUS;			//超声图像
	double m_dImageRes;			//超声图像 像素物理尺寸(mm)


	
};
