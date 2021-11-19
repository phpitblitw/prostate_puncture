//*****************************************************************
// 文件名 :						AnalyseProcess.h
// 版本	 :						1.0
// 目的及主要功能 :				实际分析处理模块
// 创建日期 :					2020.11.01
// 修改日期 :					2020.05.07
// 作者 :						Fiki
// 修改者 :						SunYizhe
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/
#pragma once

/*****************************************************************
Library Files Included
*****************************************************************/
#include <vector>
#include <thread>
#include <atomic>
#include <string>
#include "AnalyseProcess/AnalyseConfig.h"
#include "AnalyseProcess/PositionManager.h"
#include "AnalyseProcess/ImageSampler.h"
#include "AnalyseProcess/ImageProcessor.h"
#include "AnalyseProcess/FrameData.h"
#include "AnalyseProcess/RigidTransform.h"
#include "NDIOperator/NDIOperator.h"
#include "SurgicalPlan/SurgicalPlan.h"
#include "USBCapturer/USBCapturer.h"

using namespace NDIOPERATOR;
using namespace USBCAPTURER;
using namespace SURGICALPLAN;
using namespace fsutility;
using namespace std;

/*****************************************************************
Defines
*****************************************************************/
#ifdef ANALYSEPROCESS_EXPORT
#define ANALYSEPROCESS_API _declspec(dllexport)
#else
#define ANALYSEPROCESS_API _declspec(dllimport)
#endif

/*****************************************************************
						Routine Definitions
定义分析过程AnalyseProcess类
	i.	可从ini文件中读取MRI信息，包括base位置参数
	ii.	位置状态指针PositionManager，计算超声/MRI模拟采样之间的位置变换
	iii.根据MRI模拟采样的位置参数，截取2维截面
	iv.	回调函数形式返回给主函数
*****************************************************************/


namespace ANALYSEPROCESS
{
	//定义智能指针类
	class AnalyseProcess;
	typedef std::shared_ptr<AnalyseProcess> AnalyseProcessPtr;

	//用于区分当前工作状态，不同工作状态工作内容不相同
	enum AnalyzeState
	{
		INIT,					//初始化 未配准
								//此时，使用mri模拟采样base位置 来截取mask轮廓
		PENDING,				//初始化完成，调整MRI模拟采样及真实US探头位姿过程中
		REGISTERING1,			//配准计算过程中 尚未获取最新的US探头姿态
		REGISTERING2,			//配准计算过程中 已获取最新的US探头姿态
		PUNCTURE,				//穿刺	(已经配准完成)
	};
	class ANALYSEPROCESS_API AnalyseProcess
	{
	public:
		AnalyseProcess();
		~AnalyseProcess();

		int InitAnalyseProcess(CString t_strFilePathName);		//初始化分析模块
		int InitAnalyseProcess(std::string t_strFilePathName);	//初始化分析模块

		void SetNDIDevicePtr(NDIOperatorPtr t_NDIOperatorPtr);			//设置NDI设备操作指针
		void SetUSBCapturerPtr(USBCapturerPtr t_USBCapturerPtr);		//设置B超图像采集设备指针
		void SetSurgicalPlanPtr(SurgicalPlanPtr t_SurgicalPlanPtr);		//设置手术计划数据指针

		void Set2DImageSize(int t_nWidth, int t_nHeight);				//设置2D图像大小
		void Set3DSize(float t_fMaxX, float t_fMaxY, float t_fMaxZ);	//设置3D空间尺寸

		void MoveMRIRight(float distance);  //MRI mask向右(rightDir)移动
		void MoveMRIUp(float distance);  //MRI mask向上(upDir)移动
		void MoveMRIForward(float distance);  //MRI mask向前(moveDir)移动

		int	 StartAnalyse();			//启动分析函数
		int	 StopAnalyse();				//结束分析函数

		typedef std::function <void(FrameDataPtr m_FrameDataPtr) > Fun_UpdateFrameEvent;	//结束一帧分析后，将数据传至dlg
		void BindUpdateFrameEvent(Fun_UpdateFrameEvent eventFun);	//注册回调函数

		void Analyse();				//实际分析函数

		int Register();				//配准操作,由医生手动发起
		int ResetRegister();		//重置配准操作，由医生手动发起
	private:
		void UpdateNDIData(fsutility::Attitude attitude);  //更新NDI定位数据，回调函数，由NDI模块调用
		void UpdateUSBData(cv::Mat t_USBImgT, cv::Mat t_USBImgS, double dPixelSizeT, double dPixelSizeS);	//更新B超数据，回调函数，由B超模块调用

		vector<Coordinate> GetContourPts(FrameDataPtr t_FrameDataPtr);  //从单帧数据中，获取若干外轮廓点云
		void ProcessSingleFrame(FrameDataPtr t_FrameDataPtr);	//处理单帧数据
		void ProcessSingleFrameA(FrameDataPtr t_FrameDataPtr);	//处理单帧数据（未关联状态）
		void ProcessSingleFrameB(FrameDataPtr t_FrameDataPtr);	//处理单帧数据（关联状态）
		Mat Mask2Edge(Mat src);								//将mask取轮廓
	private:
		CMutex			m_ProcessDataMutex;		//显示互斥变量
		FrameDataPtr	m_CurrentFrameDataPtr;	//用于存放当前处理中的帧
		FrameDataPtr	m_NextFrameDataPtr;		//用于存放下一帧待处理的帧

		//HANDLE			m_hAnalyzeThread;	//实际处理线程
		std::thread			m_tAnalyseThread;	//分析线程
		atomic<bool>	m_bAnalysing;			//正在分析标志位

		AnalyzeState	m_nAnalyseState;		//当前分析阶段

		Fun_UpdateFrameEvent m_UpdateFrameFun;	//更新dlg数据回调函数指针

		NDIOperatorPtr		m_NDIOperatorPtr;			//NDI设备操作指针
		USBCapturerPtr		m_USBCapturerPtr;			//B超图像采集设备指针
		SurgicalPlanPtr		m_SurgicalPlanPtr;			//手术计划及数据操作指针
		PositionManagerPtr	m_PositionManagerPtr;		//位置管理指针 所有的姿态参数，都交由其进行管理和计算
		RigidTransformPtr	m_RigidTransformPtr;		//刚体变换计算对象指针
		ImageSamplerPtr		m_ImageSamplerPtr;			//切割二维平面对象指针
		ImageProcessorPtr	m_ImageProcessorPtr;		//处理2D图像工具类

		vector<Coordinate> m_USPts;				//根据US图像 截取的一组点云
		BYTE			*m_pProstateMask;		//暂存prostate mask
		BYTE			*m_pLesionMask;			//暂存lesion mask
		BYTE			*m_pRectumMask;			//暂存rectum mask

		int				m_nShowImageX;			//2D图像像素宽度	在PunctureDlg.cpp中，根据超声图像大小而设定
		int				m_nShowImageY;			//2D图像像素高度
		float			m_fMaxX;				//3D空间大小
		float			m_fMaxY;
		float			m_fMaxZ;
		
		//fsutility::Attitude m_simulatedAttitude;  //超声探头的姿态。该值用于在手动配准前，计算轮廓线
	};
}