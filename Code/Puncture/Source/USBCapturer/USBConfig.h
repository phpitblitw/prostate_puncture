//*****************************************************************
// 文件名 :						USBConfig.h
// 版本	 :						1.0
// 目的及主要功能 :				超声图像采集设备设置
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
#include "ComUtility/singleton.h"
#include "Opencv2/opencv.hpp"

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
*****************************************************************/
namespace USBCAPTURER
{
	//定义基本采集图像结构类
	class USBCAPTURER_API USBConfig : public fsutility::Singleton<USBConfig>
	{
		friend class fsutility::Singleton<USBConfig>;

	private:
		USBConfig();
		virtual ~USBConfig();

	public:
		int LoadConfigFile(CString t_strFilePathName);

	public:
		cv::Rect m_DualLeftBarRect;  //双平面的左侧长条区域
		cv::Rect m_CharSensorTypeRect;  //指示横断面(T)/矢状面(S)的区域
		cv::Rect m_OnePlaneRightAxisRect; //单平面 右侧坐标轴区域
		cv::Rect m_DualPlaneUpRightAxisRect;  //双平面模式 上方(横断面)右侧坐标轴区域
		cv::Rect m_DualPlaneDownRightAxisRect;  //双平面模式 下方(矢状面)右侧坐标轴区域
		cv::Rect m_OnePlaneROI;  //单平面模式下 图像区域
		cv::Rect m_DualUpROI;  //双平面模式下 上方(横断面)图像区域
		cv::Rect m_DualDownROI;  //双平面模式下 下方(矢状面)图像区域
	};
}
