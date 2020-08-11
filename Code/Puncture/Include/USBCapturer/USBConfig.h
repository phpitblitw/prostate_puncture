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
		//B超坐标轴区域参数
		//CvRect m_Axis;
		//右侧坐标轴坐标 用于判断分辨率
		CvRect m_RightAxisRect;
		//左侧坐标轴坐标 用于判断单平面/双平面
		CvRect m_UpAxisRect;
		CvRect m_DownAxisRect;

		//单屏幕下的参数
		CvRect m_FullRect;

		//双屏幕下的参数
		CvRect m_UpRect;
		CvRect m_DownRect;
	};
}
