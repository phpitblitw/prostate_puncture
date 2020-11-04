#pragma once

#include "ui_PunctureWindow.h"
#include <QtWidgets/QMainWindow>
#include <qtimer.h>
#include <thread>
#include <mutex>
//#include <afx.h>  //MFC支持
#include "NDIOperator/NDIOperator.h"
#include "SurgicalPlan/SurgicalPlan.h"
#include "USBCapturer/USBCapturer.h"
#include "AnalyseProcess/AnalyseProcess.h"

using namespace NDIOPERATOR;
using namespace SURGICALPLAN;
using namespace USBCAPTURER;
using namespace ANALYSEPROCESS;

class PunctureWindow : public QMainWindow
{
    Q_OBJECT

public:
    PunctureWindow(QWidget *parent = Q_NULLPTR);
	void InitWindow();  //初始化窗口

public slots:
	void OnTimerTimeout();
	int InitDevice();  //初始化设备
	void OnBtnRegisterClicked();
	void OnBtnQuitClicked();
	void MoveMRILeft();
	void MoveMRIRight();
	void MoveMRIUp();
	void MoveMRIDown();
	void MoveMRIForward();
	void MoveMRIBackward();

private:
	void UpdateFrame(FrameDataPtr t_FrameDataPtr);  //作为回调函数传递给AnalyseProcess，更新处理后的单帧数据


private:
    Ui::PunctureWindowClass ui;
	QTimer m_timer;
	bool m_b2DAcquired;  //是否已获取2D图像标志位
	bool m_b3DAcquired;  //是否已获取3D图像标志位
	bool m_bRegistered;  //是否已经配准完成
	std::mutex m_showMutex;  //显示互斥量
	FrameDataPtr		m_FrameDataPtr;			//单帧数据
	NDIOperatorPtr		m_NDIOperatorPtr;		//NDI设备操作指针
	USBCapturerPtr		m_USBCapturerPtr;		//B超图像采集设备指针
	SurgicalPlanPtr		m_SurgicalPlanPtr;		//手术计划模块指针
	AnalyseProcessPtr	m_AnalyseProcessPtr;	//分析模块指针
};
