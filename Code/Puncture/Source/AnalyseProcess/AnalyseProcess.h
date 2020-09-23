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
#include "AnalyseProcess/AnalyseConfig.h"
#include "AnalyseProcess/PositionManager.h"
#include "AnalyseProcess/ImageSampler.h"
#include "AnalyseProcess/FrameData.h"
#include "NDIOperator/NDIOperator.h"
#include "SurgicalPlan/SurgicalPlan.h"
#include "USBCapturer/USBCapturer.h"

using namespace NDIOPERATOR;
using namespace USBCAPTURER;
using namespace SURGICALPLAN;
using namespace fsutility;

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
		Init,					//初始化(尚未配准完成)
		Registration,			//配准中(尚未配准完成)
		Puncture,				//穿刺	(已经配准完成)
	};
	class ANALYSEPROCESS_API AnalyseProcess
	{
	public:
		AnalyseProcess();
		~AnalyseProcess();

		int InitAnalyseProcess(CString t_strFilePathName);	//初始化分析模块

		void SetNDIDevicePtr(NDIOperatorPtr t_NDIOperatorPtr);			//设置NDI设备操作指针
		void SetUSBCapturerPtr(USBCapturerPtr t_USBCapturerPtr);		//设置B超图像采集设备指针
		void SetSurgicalPlanPtr(SurgicalPlanPtr t_SurgicalPlanPtr);		//设置手术计划数据指针

		void Set2DImageSize(int t_nWidth, int t_nHeight);				//设置2D图像大小
		void Set3DSize(float t_fMaxX, float t_fMaxY, float t_fMaxZ);	//设置3D空间尺寸

		void StartAnalyse();			//启动分析函数
		void StopAnalyse();				//结束分析函数

		typedef std::function <void(FrameDataPtr m_FrameDataPtr) > Fun_UpdateFrameEvent;	//结束一帧分析后，将数据传至dlg
		void BindUpdateFrameEvent(Fun_UpdateFrameEvent eventFun);	//注册回调函数

		void Analyse();				//实际分析函数

		int Register();				//配准操作,由医生手动发起
	private:
		void UpdateNDIData(std::vector<Attitude> t_Attitude);						//更新NDI数据，回调函数，由NDI模块调用
		void UpdateUSBData(cv::Mat t_USBImgA, cv::Mat t_USBImgB, double m_dImageRes);	//更新B超数据，回调函数，由B超模块调用

		void ProcessSingleFrame(FrameDataPtr t_FrameDataPtr);	//处理单帧数据
		void ProcessSingleFrameA(FrameDataPtr t_FrameDataPtr);	//处理单帧数据（未关联状态）
		void ProcessSingleFrameB(FrameDataPtr t_FrameDataPtr);	//处理单帧数据（关联状态）
		Mat Mask2Edge(Mat src);								//将mask取轮廓
	private:
		CMutex			m_ProcessDataMutex;		//显示互斥变量
		FrameDataPtr	m_CurrentFrameDataPtr;	//用于存放当前处理中的帧
		FrameDataPtr	m_NextFrameDataPtr;		//用于存放下一帧待处理的帧

		HANDLE			m_hAnalyzeThread;		//实际处理线程
		AnalyzeState	m_nAnalyseState;		//当前分析阶段

		Fun_UpdateFrameEvent m_UpdateFrameFun;	//更新dlg数据回调函数指针

		NDIOperatorPtr		m_NDIOperatorPtr;			//NDI设备操作指针
		USBCapturerPtr		m_USBCapturerPtr;			//B超图像采集设备指针
		SurgicalPlanPtr		m_SurgicalPlanPtr;			//手术计划及数据操作指针
		PositionManagerPtr	m_PositionManagerPtr;		//位置管理指针
		ImageSamplerPtr		m_ImageSamplerPtr;			//切割二维平面对象指针

		BYTE			*m_pProstateMask;		//暂存prostate mask
		BYTE			*m_pLesionMask;			//暂存lesion mask
		BYTE			*m_pRectumMask;			//暂存rectum mask

		int				m_nShowImageX;			//2D图像像素宽度	在PunctureDlg.cpp中，根据超声图像大小而设定
		int				m_nShowImageY;			//2D图像像素高度
		float			m_fMaxX;				//3D空间大小
		float			m_fMaxY;
		float			m_fMaxZ;
		
	};
}