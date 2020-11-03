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
#include <atomic>
#include <string>
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
	//定义B超机扫描模式
	enum ScanType
	{
		UNDEFINED = 0,  //扫描类型未确定
		TRANSVERSE_ONLY = 1,  //仅横断面
		SAGITTAL_ONLY = 2, //仅矢状面
		DUAL_PLANE = 3  //双平面
	};

	//定义智能指针类
	class USBCapturer;
	typedef std::shared_ptr<USBCapturer> USBCapturerPtr;

	//定义基本采集图像结构类
	class USBCAPTURER_API USBCapturer
	{
	public:
		USBCapturer();
		virtual ~USBCapturer();

	public:
		int InitUSBDevice(CString t_strFilePathName);		//初始化B超图像采集设备
		int InitUSBDevice(std::string t_strFilePathName);	//初始化B超图像采集设备
		int ReleaseUSBDevice();		//释放B超图像采集设备

		int StartGrab();		//开始采集B超数据
		int StopGrab();			//停止采集B超数据
		BOOL GrabOneFrame(Mat &t_Image);		//获取一帧B超图像

		//传递数据至外部回调函数
		typedef std::function < void(cv::Mat, cv::Mat, double, double) > Fun_CapturePerFrameEvent;		//支持两个图像和尺度信息
		void BindCapturePerFrameEvent(Fun_CapturePerFrameEvent eventFun);  //绑定刷新坐标回调函数

		void Grab(); //持续采集B超数据

		

	private:
		Fun_CapturePerFrameEvent m_CapturePerFrameFun;		//更新预览图
		void CalParameters(cv::Mat t_img);  //对于CV_8UC3格式的Mat图片 更新b超参数:扫描类型  像素物理尺寸  TODO
		ScanType CalScanType(cv::Mat t_img);  //对于CV_8UC3格式的Mat图片，计算其对应b超扫描平面类型  TODO
		double CalPixelSize(cv::Mat t_imgAxis);  //对于CV_8UC3格式的Mat图片，计算其像素大小，即1个像素对应的物理尺寸(mm)  TODO

	private:
		cv::Mat m_imgCharT;  //字母T的截图(8UC1) 用于比对 确定单平面类型是否为Transverse横截面
		cv::Mat m_imgCharS;  //字母S的截图(8UC1) 用于比对 确定单平面类型是否为Sagittal矢状面
		vector <cv::Point2f> m_NeedlePos;			//穿刺架投影点坐标，从ini文件中读出

		FrmGrabber * m_pDevice;						//采集卡设备对象指针

		atomic<bool> m_bRunning;					//正在采集状态标志位
		std::thread m_tGrabThread;						//B超采集线程
		std::mutex m_CaptureMutex;					//采集标志位操作互斥量

		ScanType m_scanType;  //b超机的扫描类型
		double m_dPixelSizeT;  //横截面的像素物理尺寸
		double m_dPixelSizeS;  //矢状面的像素物理尺寸
	};
}