//*****************************************************************
// 文件名 :						ImageProcessor.h
// 版本	 :						1.0
// 目的及主要功能 :				处理2D图像工具类
// 创建日期 :					2021.11.12
// 修改日期 :					2021.11.12
// 作者 :						SunYizhe
// 修改者 :						
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/
#pragma once

/*****************************************************************
Library Files Included
*****************************************************************/
#include "opencv2/opencv.hpp"

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
处理2D图像 工具类
	i.	对2D横断面US图像 分割前列腺区域
*****************************************************************/

namespace ANALYSEPROCESS
{
	class ImageProcessor;
	typedef std::shared_ptr<ImageProcessor> ImageProcessorPtr;

	class ImageProcessor
	{
	public:
		ImageProcessor();
		~ImageProcessor();

		static cv::Mat SegmentByThreshold(cv::Mat &srcImg);  //对于横断面图像，分割前列腺(0/255 二值图)
	private:

	};
}



