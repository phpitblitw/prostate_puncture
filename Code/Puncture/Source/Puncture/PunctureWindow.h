#pragma once
#include <QtWidgets/QMainWindow>
#include <qtimer.h>
#include <thread>
#include <mutex>
#include "NDIOperator/NDIOperator.h"
#include "SurgicalPlan/SurgicalPlan.h"
#include "USBCapturer/USBCapturer.h"
#include "AnalyseProcess/AnalyseProcess.h"
#include "ui_PunctureWindow.h"  //由于https://www.cnblogs.com/time-is-life/p/5436633.html所述的原因，要把mfc涉及的#include <windows.h>放在前面，其他文件(QOpenglWidget3D.h)包含的include<windows.h>放在后面

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

	void closeEvent(QCloseEvent * event);

public slots:
	void OnTimerTimeout();
	int InitDevice();  //初始化设备
	int LoadPatientData();  //载入病人数据
	void OnBtnRegisterClicked();  //超声探头-MRI模拟采样 配准(仿射变换)
	void OnBtnUpdateUSClicked();  //更新超声设备参数
	void Quit();  //释放设备并退出
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
