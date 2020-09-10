//*****************************************************************
// 文件名 :						AnalyzeProcess.h
// 版本	 :						1.0
// 目的及主要功能 :				实际分析处理模块
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					
//*****************************************************************/
#pragma once

/*****************************************************************
Library Files Included
*****************************************************************/
#include <vector>
#include "NDIOperator/NDIOperator.h"
#include "SurgicalPlan/SurgicalPlan.h"
#include "USBCapturer/USBCapturer.h"
#include "AnalyseProcess/FrameData.h"
#include "SurgicalPlan/ImageSampler.h"


using namespace NDIOPERATOR;
using namespace SURGICALPLAN;
using namespace USBCAPTURER;


/*****************************************************************
Defines
*****************************************************************/


/*****************************************************************
						Routine Definitions
*****************************************************************/
//实际处理模块
class AnalyzeProcess;
typedef std::shared_ptr<AnalyzeProcess> AnalyzeProcessPtr;

//用于区分当前工作状态，不同工作状态工作内容不相同
enum AnalyzeState
{
	Init,					//初始化
	Registration,			//配准
	Puncture,				//穿刺
};

class AnalyzeProcess
{
public:
	AnalyzeProcess();
	virtual ~AnalyzeProcess();

	void StartAnalyze();			//启动分析函数
	void StopAnalyze();				//结束分析函数
	
	typedef std::function <void(FrameDataPtr m_FrameDataPtr) > Fun_UpdateFrameEvent;	//结束一帧分析后，将数据传至dlg
	void BindUpdateFrameEvent(Fun_UpdateFrameEvent eventFun);	//注册回调函数

	void SetNDIDevicePtr(NDIOperatorPtr t_NDIOperatorPtr);			//设置NDI设备操作指针
	void SetUSBCapturerPtr(USBCapturerPtr m_USBCapturerPtr);		//设置B超图像采集设备指针

	void Set2DImageSize(int t_nWidth, int t_nHeight);				//设置2D图像大小
	void Set3DSize(float t_fMaxX, float t_fMaxY, float t_fMaxZ);	//设置3D空间尺寸

	SurfaceDataPtr GetSurfaceDataPtr();								//返回表面模型
	void GetCenterPt(double &x, double &y, double &z);				//得到中心坐标
	//void GetParameters(double &t_centerX, double &t_centerY, double &t_centerZ,
	//	double &t_MX, double &t_MY, double &t_MZ,
	//	double &t_UX, double &t_UY, double &t_UZ);

	void CalibratePlanUSBCoordinate();								//计算超声探头与手术计划数据之间的转换矩阵

	int InportData(CString t_strFilePathName);				//导入手术计划数据

	void Analyze();				//实际分析函数

private:
	void UpdateNDIData(std::vector<Attitude> t_Attitude);						//更新NDI数据，回调函数，由NDI模块调用
	void UpdateUSBData(cv::Mat t_USBImgA, cv::Mat t_USBImgB, int t_nScale);		//更新USB数据，回调函数，由NDI模块调用

	void ProcessSingleFrame(FrameDataPtr t_FrameDataPtr);	//处理单帧数据
	void ProcessSingleFrameA(FrameDataPtr t_FrameDataPtr);	//处理单帧数据（未关联状态）
	void ProcessSingleFrameB(FrameDataPtr t_FrameDataPtr);	//处理单帧数据（关联状态）

	int InportTransMatrixN_U(CString m_strFileName);		//导入NDI——B超坐标转换矩阵

public:

private:
	CMutex			m_ProcessDataMutex;		//显示互斥变量
	FrameDataPtr	m_CurrentFrameDataPtr;	//用于存放当前处理中的帧
	FrameDataPtr	m_NextFrameDataPtr;		//用于存放下一帧待处理的帧

	HANDLE			m_AnalyzeThread;		//实际处理线程

	AnalyzeState	m_nAnalyzeState;		//当前分析阶段
	bool			m_bStopAnalyze;			//停止分析，用于停止软件工作

	Fun_UpdateFrameEvent m_UpdateFrameFun;	//更新dlg数据回调函数指针

	NDIOperatorPtr	m_NDIOperatorPtr;		//NDI设备操作指针
	USBCapturerPtr	m_USBCapturerPtr;		//B超图像采集设备指针

	SurgicalPlanPtr	m_SurgicalPlanPtr;		//手术计划及数据操作指针
	ImageSamplerPtr	m_pMaskSampler;			//采样图像？

	cv::Mat			m_TransMatrixPlanUSB;	//超声探头与手术计划数据之间的转换矩阵

	int				m_nShowImageX;			//2D图像像素宽度
	int				m_nShowImageY;			//2D图像像素高度
		
	float			m_fMaxX;				//3D空间大小
	float			m_fMaxY;
	float			m_fMaxZ;	

	vector<Mat>		m_vTransrMatrixN_U;		//NID_超声坐标转换矩阵，为应对多组不同分辨率，所以用vector存储
};