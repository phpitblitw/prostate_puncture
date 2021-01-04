#pragma once

#include <mutex>
#include <QtWidgets/QMainWindow>
#include <qtimer.h>
#include "NDIOperator/NDIOperator.h"
#include "USBCapturer/USBCapturer.h"
#include "AnnotateWindow.h"
#include "LocatePhantomDlg.h"
#include "ui_CalibrateWindow.h"
using namespace NDIOPERATOR;
using namespace USBCAPTURER;

class CalibrateWindow : public QMainWindow
{
    Q_OBJECT

public:
    CalibrateWindow(QWidget *parent = Q_NULLPTR);
	~CalibrateWindow();
	void closeEvent();

private:
	void UpdateUSImage(cv::Mat, cv::Mat, double, double);  //作为回调函数传给USCapturer,更新超声数据
	void UpdateNDIData(NDIOPERATOR::Attitude);  //作为回调函数传给NDIOperator,更新NDI数据(欧拉角形式)
private slots:
	int OnBtnInitDeviceClicked();
	void OnBtnStartCaptureClicked();
	void OnBtnAnnotateClicked();
	void OnBtnLocatePhantomClicked();
	void ReleaseDevice();  //关闭设备
	void OnTimerDisplay();  //定时刷新显示
	void OnTimerCapture();  //定时采集
private:
    Ui::CalibrateWindowClass ui;
	AnnotateWindow * m_AnnotateWindowPtr;  //用于标注图片上的点
	LocatePhantomDlg * m_LocatePhantomDlgPtr;  //用于定位体模的几个角点
	QTimer m_timerDisplay;  //刷新显示计时器
	QTimer m_timerCapture;  //采集数据计时器
	NDIOperatorPtr		m_NDIOperatorPtr;		//NDI设备操作指针
	USBCapturerPtr		m_USBCapturerPtr;		//B超图像采集设备指针
	cv::Mat m_imgUS;  //超声图像
	float m_fUSRes;  //超声图像分辨率(一个像素的物理尺寸 mm/pixel)
	NDIOPERATOR::Attitude m_euler;  //超声探头姿态(欧拉角形式)
	bool m_bUSAcquired;  //超声图像已获得标志位
	int m_nDataIndex;  //采集、待存储的数据下标
	std::mutex m_showMutex;  //显示、数据更新 互斥量
	string m_strDataDir;  //采集数据存储的位置

};
