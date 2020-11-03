//*****************************************************************
// 文件名 :						FrameData.h
// 版本	 :						2.0
// 目的及主要功能 :				单帧信息类
// 创建日期 :					2019.11.1
// 修改日期 :					2010.05.07
// 作者 :						Fiki
// 修改者 :						SunYizhe
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/
#pragma once


/*****************************************************************
Project Files Included
*****************************************************************/
#include <memory>
#include "Opencv2/opencv.hpp"
#include "NDIOperator/Attitude.h"
#include "ComUtility/Coordinate.h"
#include "ComUtility/Attitude.h"

//using namespace cv;
using namespace NDIOPERATOR;
using namespace fsutility;

/*****************************************************************
Defines
单帧截面FrameData类
	i.	记录某一时刻的截面参数类
	ii.	包括由超声设备获取的超声图像
	iii.包括由MRI模拟采样 截取出的2维MRI 前列腺/病灶截面
*****************************************************************/
#ifdef ANALYSEPROCESS_EXPORT
#define ANALYSEPROCESS_API _declspec(dllexport)
#else
#define ANALYSEPROCESS_API _declspec(dllimport)
#endif


/*****************************************************************
						Routine Definitions
*****************************************************************/

namespace ANALYSEPROCESS
{
	//单帧信息类
	class FrameData;
	typedef std::shared_ptr<FrameData> FrameDataPtr;

	class ANALYSEPROCESS_API FrameData
	{
	public:
		FrameData();
		virtual ~FrameData();

		//int	CreatMaskData(int t_nShowWidth, int t_nShowHeight);		//开辟2D掩模图像空间
		void SetPosition(fsutility::Attitude Attitude);	//设置当前切片MRI模拟采样姿态参数(wld)
		//void SetPosition(Coordinate ScanCenter, Coordinate RightDir, Coordinate UpDir, Coordinate MoveDir);	//设置当前切片MRI模拟采样姿态参数(wld)

	public:
		//Transverse横断面数据
		cv::Mat		m_USImgT;  //超声图像
		cv::Mat		m_prostateMaskT;  //mask数据 CV_8UC1
		cv::Mat		m_lesionMaskT;
		cv::Mat		m_rectumMaskT;
		double		m_dPixelSizeT;  //像素物理尺寸(mm)

		//Sagittal矢状面数据
		cv::Mat		m_USImgS;  //超声图像
		cv::Mat		m_prostateMaskS;  //mask数据 CV_8UC1
		cv::Mat		m_lesionMaskS;
		cv::Mat		m_rectumMaskS;
		double		m_dPixelSizeS;  //像素物理尺寸(mm)

		//int			m_nScale;		//当前b超图像的比例尺
		//double m_dImageRes;		//缩放比例，即1个像素对应的物理尺寸(mm)

		//Attitude	m_USBPos;		//超声探头的空间位置
		//Attitude	m_NeedlePos;	//探针空间位置（预留，待后期完善）

		//BYTE *m_pProstateMask;		//前列腺2D掩模图像
		//BYTE *m_pLesionMask;		//病灶2D掩模图像
		//BYTE *m_pFusionMask;		//总的2Dmask轮廓图像，1-前列腺，2-病灶，3-直肠

		////MRI模拟采样 截面的姿态参数 (wld)
		//Coordinate m_ScanCenter;
		//Coordinate m_RightDir;
		//Coordinate m_UpDir;
		//Coordinate m_MoveDir;

		//MRI模拟采样 截面的姿态参数 (wld)
		fsutility::Attitude m_Attitude;

		//MRI模拟采样 截面的四个角点(wld)
		Coordinate m_LeftTop;
		Coordinate m_RightTop;
		Coordinate m_LeftBottom;
		Coordinate m_RightBottom;
	};
}

