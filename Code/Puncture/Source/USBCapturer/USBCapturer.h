//*****************************************************************
// 文件名 :						USBCapturer.h
// 版本	 :						1.0
// 目的及主要功能 :				超声图像采集设备控制
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/
#pragma once


/*****************************************************************
Project Files Included
*****************************************************************/
#include <memory>
#include <vector>
#include <functional>
#include "frmgrab.h"
#include <thread>
#include <mutex>
#include "Opencv2/opencv.hpp"

using namespace std;
using namespace cv;


/*****************************************************************
Defines
*****************************************************************/
#ifdef USBCAPTURER_EXPORT
#define USBCAPTURER_API __declspec(dllexport)
#else
#define USBCAPTURER_API __declspec(dllimport)
#endif


/*****************************************************************
						Routine Definitions

定义采集卡类Capturer类
	i.	设备操作指针；
	ii.	基本图像信息，宽、高、帧率；
	iii.	回调函数形式读取单帧数据；
	iv.	需要对图像进行剪裁；
	v.	可从ini文件中读取基本设置信息，包括采集卡信息和区域裁剪信息；

*****************************************************************/
namespace USBCAPTURER
{
	//定义智能指针类
	class USBCapturer;
	typedef std::shared_ptr<USBCapturer> USBCapturerPtr;
	UINT GrabThreadFun(LPVOID lpParam);//持续采集B超数据（不是类的成员函数，供CreateThread调用)

	//定义基本采集图像结构类
	class USBCAPTURER_API USBCapturer
	{
	public:
		USBCapturer();
		virtual ~USBCapturer();

	public:
		int InitUSBDevice(CString t_strFilePathName);		//初始化B超图像采集设备
		int ReleaseUSBDevice();		//释放B超图像采集设备

		int StartGrab();		//开始采集B超数据
		int StopGrab();			//停止采集B超数据
		BOOL GrabOneFrame(Mat &t_Image);		//获取一帧B超图像

		//传递数据至外部回调函数
		typedef std::function < void(cv::Mat, cv::Mat, double) > Fun_CapturePerFrameEvent;		//支持两个图像和尺度信息
		void BindCapturePerFrameEvent(Fun_CapturePerFrameEvent eventFun);  //绑定刷新坐标回调函数

		double GetResolution(); //获取缩放比例
		void GetImageSize(int &cx, int &cy);	//返回图片尺寸(由单/双平面确定)
		void Grab(); //持续采集B超数据

		

	private:
		Fun_CapturePerFrameEvent m_CapturePerFrameFun;		//更新预览图

	private:
		vector <cv::Point2f> m_NeedlePos;			//穿刺架投影点坐标，从ini文件中读出

		FrmGrabber * m_pDevice;						//采集卡设备对象指针

		volatile bool m_bRunning;					//正在采集状态标志位
		std::mutex m_CaptureMutex;					//采集标志位操作互斥量

		HANDLE m_hGrabThread; //B超采集线程句柄
		DWORD m_nGrabThreadID; //B超采集线程id

		double m_dImageRes;		//缩放比例，即1个像素对应的物理尺寸(mm)
		int m_nScreenType;		//屏显种类 1-单平面 2-双平面
		bool m_bGetParameters;	//已经采集参数标志位
		void CalParameters(Mat t_Image);
		double CalResolution(Mat t_Image);
		int CalScreenType(Mat t_Image);
	};
}