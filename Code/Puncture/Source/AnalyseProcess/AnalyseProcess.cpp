//*****************************************************************
// 文件名 :						AnalyseProcess.cpp
// 版本	 :						1.0
// 目的及主要功能 :				实际分析处理模块
// 创建日期 :					2020.11.01
// 修改日期 :					2020.05.07
// 作者 :						Fiki
// 修改者 :						SunYizhe
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "pch.h"
#include "AnalyseProcess.h"
#include "ErrorManager/ErrorCodeDefine.h"

using namespace ANALYSEPROCESS;

///////////////////////////////////////////////////////
////////////////////////define/////////////////////////
///////////////////////////////////////////////////////

/*****************************************************************
Global Variables
*****************************************************************/

/*****************************************************************
Function
*****************************************************************/
/*****************************************************************
Name:			AnalyzeThread
Inputs:
	AnalyseProcess* lpParam - 分析对象指针
Return Value:
	none
Description:	分析多线程
*****************************************************************/
DWORD WINAPI AnalyseThread(LPVOID lpParam)			//分析线程
{
	AnalyseProcess *t_pPoint;
	t_pPoint = (AnalyseProcess *)lpParam;

	t_pPoint->Analyse();		//启动分析

	return 1;
}//AnalyzeThread


/*****************************************************************
Name:			AnalyzeProcess
Inputs:
	none
Return Value:
	none
Description:	AnalyzeProcess构造函数
*****************************************************************/
AnalyseProcess::AnalyseProcess()
{
	//m_SurgicalPlanPtr.reset(new SurgicalPlan());		//手术计划
	m_PositionManagerPtr.reset(new PositionManager());	//位置管理
	m_ImageSamplerPtr.reset(new ImageSampler());		//切割截面
	m_nShowImageX = 0;
	m_nShowImageY = 0;
	m_fMaxX = 0;
	m_fMaxY = 0;
	m_fMaxZ = 0;
	m_NextFrameDataPtr.reset(new FrameData);
	m_CurrentFrameDataPtr = nullptr;
	m_nAnalyseState = Init;
	m_pProstateMask = nullptr;
	m_pLesionMask = nullptr;
	m_pRectumMask = nullptr;
}//AnalyzeProcess


/*****************************************************************
Name:			~AnalyzeProcess
Inputs:
	none
Return Value:
	none
Description:	AnalyzeProcess析构函数
*****************************************************************/
AnalyseProcess::~AnalyseProcess()
{
}//~AnalyzeProcess


/*****************************************************************
Name:			InitAnalyseProcess
Inputs:
	none
Return Value:
	int - error info
Description:	初始化分析模块
*****************************************************************/
int AnalyseProcess::InitAnalyseProcess(CString t_strFilePathName)
{
	//从config文件中，读取术前MRI数据分析结果
	if (AnalyseConfig::Instance().LoadConfigFile(t_strFilePathName) != LIST_NO_ERROR)
	{
		return ER_OpenAnalyseConfigFileFailed;
	}

	//设置ImageSampler类
	if (m_ImageSamplerPtr->SetDataPackage(m_SurgicalPlanPtr) != LIST_NO_ERROR)
	{
		return ER_NoSurgicalPlan;
	}
	m_ImageSamplerPtr->SetMRIPixelSize(AnalyseConfig::Instance().m_fVoxelSizeX, AnalyseConfig::Instance().m_fVoxelSizeY, AnalyseConfig::Instance().m_fVoxelSizeZ);
	return LIST_NO_ERROR;
}//InitAnalyseProcess


/*****************************************************************
Name:			SetNDIDevicePtr
Inputs:
	NDIOperatorPtr t_NDIOperatorPtr - NDI设备指针
Return Value:
	none
Description:	设置NDI设备操作指针
*****************************************************************/
void AnalyseProcess::SetNDIDevicePtr(NDIOperatorPtr t_NDIOperatorPtr)
{
	m_NDIOperatorPtr = t_NDIOperatorPtr;

	m_NDIOperatorPtr->BindUpdateAttitudeEvent(std::bind(&AnalyseProcess::UpdateNDIData, this, std::placeholders::_1));
}//SetNDIDevicePtr


/*****************************************************************
Name:			SetUSBCapturerPtr
Inputs:
	USBCapturerPtr t_USBCapturerPtr - 超声图像获取设备指针
Return Value:
	none
Description:	设置超声图像获取设备操作指针
*****************************************************************/
void AnalyseProcess::SetUSBCapturerPtr(USBCapturerPtr t_USBCapturerPtr)
{
	m_USBCapturerPtr = t_USBCapturerPtr;

	m_USBCapturerPtr->BindCapturePerFrameEvent(std::bind(&AnalyseProcess::UpdateUSBData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}//SetUSBCapturerPtr


/*****************************************************************
Name:			SetSurgicalPlanPtr
Inputs:
	SurgicalPlanPtr t_SurgicalPlanPtr - 手术计划数据指针
Return Value:
	none
Description:	设置手术计划数据指针
*****************************************************************/
void AnalyseProcess::SetSurgicalPlanPtr(SurgicalPlanPtr t_SurgicalPlanPtr)
{
	m_SurgicalPlanPtr = t_SurgicalPlanPtr;
}//SetSurgicalPlanPtr

/*****************************************************************
Name:			Set2DImageSize
Inputs:
	int t_nWidth - 2D图像宽度
	int t_nHeight - 2D图像高度
Return Value:
	none
Description:	设置2D图像尺寸
*****************************************************************/
void AnalyseProcess::Set2DImageSize(int t_nWidth, int t_nHeight)
{
	if (t_nWidth <= 50 || t_nHeight <= 50
		|| t_nWidth > 3000 || t_nHeight > 3000)
	{
		return;
	}

	m_nShowImageX = t_nWidth;
	m_nShowImageY = t_nHeight;
}//Set2DImageSize


/*****************************************************************
Name:			Set3DSize
Inputs:
	float t_fMaxX - 数据区高度
	float t_fMaxY - 数据区宽度
	float t_fMaxZ - 数据区深度
Return Value:
	none
Description:	设置3D图像尺寸
*****************************************************************/
void AnalyseProcess::Set3DSize(float t_fMaxX, float t_fMaxY, float t_fMaxZ)
{
	if (t_fMaxX <= 50 || t_fMaxX > 1000
		|| t_fMaxY <= 50 || t_fMaxY > 1000
		|| t_fMaxZ <= 20 || t_fMaxZ > 1000)
	{
		return;
	}

	m_fMaxX = t_fMaxX;
	m_fMaxY = t_fMaxY;
	m_fMaxZ = t_fMaxZ;
}//Set3DSize


/*****************************************************************
Name:			StartAnalyze
Inputs:
	none
Return Value:
	none
Description:	启动分析函数，创建分析线程
*****************************************************************/
void AnalyseProcess::StartAnalyse()
{
	m_hAnalyzeThread = CreateThread(nullptr, 0, AnalyseThread, this, 0, nullptr);
	return;
}//StartAnalyze


/*****************************************************************
Name:			StopAnalyze
Inputs:
	none
Return Value:
	none
Description:	结束分析函数，修改标志
*****************************************************************/
void AnalyseProcess::StopAnalyse()
{
	m_USBCapturerPtr->StopGrab();
	m_NDIOperatorPtr->StopTracking();
	CloseHandle(m_hAnalyzeThread);
	m_nAnalyseState = Init;
	return;
}//StopAnalyze


/*****************************************************************
Name:			BindUpdateFrameEvent
Inputs:
	none
Return Value:
	none
Description:	绑定回调函数
*****************************************************************/
void AnalyseProcess::BindUpdateFrameEvent(Fun_UpdateFrameEvent eventFun)
{
	m_UpdateFrameFun = eventFun;
	return;
}

/*****************************************************************
Name:			Analyze
Inputs:
	none
Return Value:
	none
Description:	实际分析函数
*****************************************************************/
void AnalyseProcess::Analyse()
{
	while (1)
	{
		CSingleLock singlelock(&m_ProcessDataMutex);
		singlelock.Lock();
		//如果有待处理的，来自B超采集模块的图像数据，则计算对应的MRI模拟采样，生成对应的FrameData
		if (!m_NextFrameDataPtr->m_USBImage.empty())
		{
			m_CurrentFrameDataPtr = m_NextFrameDataPtr;
			m_NextFrameDataPtr.reset(new FrameData);
			singlelock.Unlock();
		}
		//若果没有待处理的B超图像数据，则不作处理
		else
		{
			Sleep(15);
			singlelock.Unlock();
			continue;
		}

		ProcessSingleFrame(m_CurrentFrameDataPtr);		//处理单帧数据
	}
}


/*****************************************************************
Name:			Register
Inputs:
	none
Return Value:
	none
Description:	配准操作,计算变换矩阵。由医生手动发起
*****************************************************************/
int AnalyseProcess::Register()
{
	CSingleLock singlelock(&m_ProcessDataMutex);
	//设置分析状态为Registration(正在配准)
	m_nAnalyseState = Registration;
	//在配准前，默认NDI设备已经采集到当前超声探头的位置
	//将MRI模拟采样的base位置传递给PositionManager
	m_PositionManagerPtr->m_BaseMRIScanCenter = AnalyseConfig::Instance().m_ScanCenter;
	m_PositionManagerPtr->m_BaseMRIRightDir = AnalyseConfig::Instance().m_RightDir;
	m_PositionManagerPtr->m_BaseMRIUpDir = AnalyseConfig::Instance().m_UpDir;
	m_PositionManagerPtr->m_BaseMRIMoveDir = AnalyseConfig::Instance().m_MoveDir;
	//将当前超声位置作为超声base位置，记录在PositionManager中
	singlelock.Lock();
	m_PositionManagerPtr->m_BaseUSScanCenter = m_PositionManagerPtr->m_CurUSScanCenter;
	m_PositionManagerPtr->m_BaseUSRightDir = m_PositionManagerPtr->m_CurUSRightDir;
	m_PositionManagerPtr->m_BaseUSUpDir = m_PositionManagerPtr->m_CurUSUpDir;
	m_PositionManagerPtr->m_BaseUSMoveDir = m_PositionManagerPtr->m_CurUSMoveDir;
	singlelock.Unlock();
	//由 US的base位置、MRI模拟采样的base位置，计算变换矩阵
	if (m_PositionManagerPtr->CalculateTransformMatrix() != LIST_NO_ERROR)
	{
		return ER_CalculateTransformMatrix;
	}
	else
	{
		m_nAnalyseState = Puncture;
		return LIST_NO_ERROR;
	}
}


/*****************************************************************
Name:			UpdateNDIData
Inputs:
	none
Return Value:
	none
Description:	由NDI数据更新超声位置参数，回调函数，由NDI模块调用。
*****************************************************************/
void AnalyseProcess::UpdateNDIData(std::vector<Attitude> t_Attitude)
{
	//现在默认处理第一个NDI位置
	CSingleLock singlelock(&m_ProcessDataMutex);
	singlelock.Lock();
	m_PositionManagerPtr->SetCurUSPosition(t_Attitude[0]);
	singlelock.Unlock();
	return;
}//UpdateNDIData


/*****************************************************************
Name:			UpdateUSBData
Inputs:
	cv::Mat t_USBImgA - 全图或上半区图像
	cv::Mat t_USBImgB - 下半区图像，可能为空
	int m_dImageRes - 缩放比例
Return Value:
	none
Description:	更新USB数据，回调函数，由US模块调用
*****************************************************************/
void AnalyseProcess::UpdateUSBData(cv::Mat t_USBImgA, cv::Mat t_USBImgB, double m_dImageRes)
{
	CSingleLock singlelock(&m_ProcessDataMutex);
	singlelock.Lock();
	m_NextFrameDataPtr->m_USBImage = t_USBImgA;
	m_NextFrameDataPtr->m_dImageRes = m_dImageRes;
	
	m_ImageSamplerPtr->SetUSPixelSize(m_dImageRes);	//设置B超图像像素大小(一个像素的物理尺寸)
	m_ImageSamplerPtr->SetImageSize(t_USBImgA.cols, t_USBImgA.rows);	//设置B超图像大小(像素数)
	singlelock.Unlock();
}//UpdateUSBData


/*****************************************************************
Name:			ProcessSingleFrame
Inputs:
	none
Return Value:
	none
Description:	处理单帧数据
*****************************************************************/
void AnalyseProcess::ProcessSingleFrame(FrameDataPtr t_FrameDataPtr)
{
	//根据当前分析状态(已配准/未配准),调用次一级分析函数
	if (m_nAnalyseState == Puncture)
	{
		ProcessSingleFrameB(t_FrameDataPtr);	//已配准状态，调用分析函数 计算截面
	}
	else
	{
		ProcessSingleFrameA(t_FrameDataPtr);	//未配准状态，截面返回默认值
	}
	//将分析结果发送给dlg
	if (m_UpdateFrameFun != nullptr)
	{
		m_UpdateFrameFun(t_FrameDataPtr);
	}
	return;
}


/*****************************************************************
Name:			ProcessSingleFrameA
Inputs:
	none
Return Value:
	none
Description:	处理单帧数据(Init或Registration，未关联状态)
*****************************************************************/
void AnalyseProcess::ProcessSingleFrameA(FrameDataPtr t_FrameDataPtr)
{
	CSingleLock singlelock(&m_ProcessDataMutex);

	//开辟mask空间
	singlelock.Lock();
	if (t_FrameDataPtr->CreatMaskData(m_nShowImageX, m_nShowImageY) != LIST_NO_ERROR)
	{
		return;
	}
	//未关联状态，无需进行MRI模拟采样，将MASK直接置为全0
	//TODO，设置空的MRI原始数据截面
	memset(t_FrameDataPtr->m_pFusionMask, 0, sizeof(BYTE)*m_nShowImageX*m_nShowImageY);
	//memset(t_FrameDataPtr->m_pLesionMask, 0, sizeof(BYTE)*m_nShowImageX*m_nShowImageY);
	//memset(t_FrameDataPtr->m_pProstateMask, 0, sizeof(BYTE)*m_nShowImageX*m_nShowImageY);
	singlelock.Unlock();
	return;
}


/*****************************************************************
Name:			ProcessSingleFrameB
Inputs:
	none
Return Value:
	none
Description:	处理单帧数据(Puncture，已关联状态)
*****************************************************************/
void AnalyseProcess::ProcessSingleFrameB(FrameDataPtr t_FrameDataPtr)
{
	//共享数据锁
	CSingleLock singlelock(&m_ProcessDataMutex);

	//开辟mask空间
	singlelock.Lock();
	if (t_FrameDataPtr->CreatMaskData(m_nShowImageX, m_nShowImageY) != LIST_NO_ERROR)
	{
		return;
	}

	//计算当前截面位置
	m_PositionManagerPtr->UpDate();	//根据已经获取的超声探头位置参数，更新MRI模拟采样位置参数
	m_ImageSamplerPtr->SetPosition(m_PositionManagerPtr->m_CurMRIScanCenter, m_PositionManagerPtr->m_CurMRIRightDir, m_PositionManagerPtr->m_CurMRIUpDir, m_PositionManagerPtr->m_CurMRIMoveDir);	//为ImageSampler设置位置参数

	//更新当前截面姿态(借用ImageSampler的函数) TODO
	//t_FrameDataPtr->SetPosition(m_ImageSamplerPtr->WLDToIJK(m_PositionManagerPtr->m_CurMRIScanCenter),
	//	m_ImageSamplerPtr->WLDToIJK(m_PositionManagerPtr->m_CurMRIRightDir),
	//	m_ImageSamplerPtr->WLDToIJK(m_PositionManagerPtr->m_CurMRIUpDir),
	//	m_ImageSamplerPtr->WLDToIJK(m_PositionManagerPtr->m_CurMRIMoveDir));
	t_FrameDataPtr->SetPosition(m_PositionManagerPtr->m_CurMRIScanCenter,
		m_PositionManagerPtr->m_CurMRIRightDir,
		m_PositionManagerPtr->m_CurMRIUpDir,
		m_PositionManagerPtr->m_CurMRIMoveDir);

	//更新当前截面4个角点的位置(wld)
	m_ImageSamplerPtr->GetPlaneCorners(t_FrameDataPtr->m_LeftTop, t_FrameDataPtr->m_RightTop, t_FrameDataPtr->m_LeftBottom, t_FrameDataPtr->m_RightBottom);

	//裁剪3类mask截面
	if (m_pProstateMask == nullptr)
		m_pProstateMask = new BYTE[m_nShowImageX*m_nShowImageY];
	if (m_pLesionMask == nullptr)
		m_pLesionMask = new BYTE[m_nShowImageX*m_nShowImageY];
	if (m_pRectumMask == nullptr)
		m_pRectumMask = new BYTE[m_nShowImageX*m_nShowImageY];
	m_ImageSamplerPtr->GetSampleMaskPlan(m_pProstateMask, 0, 1);
	////测试代码TODO
	//int frontSum = 0;
	//for (int i = 0; i < 800 * 700; i++)
	//	frontSum += m_pProstateMask[i];
	////测试代码TODO
	m_ImageSamplerPtr->GetSampleMaskPlan(m_pLesionMask, 0, 2);
	m_ImageSamplerPtr->GetSampleMaskPlan(m_pRectumMask, 0, 3);
	//mask转为轮廓
	Mat prostateContour(m_nShowImageY, m_nShowImageX, CV_8UC1, m_pProstateMask);
	Mat lesionContour(m_nShowImageY, m_nShowImageX, CV_8UC1, m_pLesionMask);
	Mat rectumContour(m_nShowImageY, m_nShowImageX, CV_8UC1, m_pRectumMask);
	prostateContour = Mask2Edge(prostateContour);
	lesionContour = Mask2Edge(lesionContour);
	rectumContour = Mask2Edge(rectumContour);
	//融合为FusionMask
	Mat fusionContour;
	addWeighted(prostateContour, 1, lesionContour, 2, 0, fusionContour);
	addWeighted(fusionContour, 1, rectumContour, 3, 0, fusionContour);
	//imwrite("D:\\fusionMask.bmp", fusionContour);	//测试代码TODO
	//交付FrameData
	memcpy(t_FrameDataPtr->m_pFusionMask, fusionContour.data, m_nShowImageX*m_nShowImageY * sizeof(BYTE));
	singlelock.Unlock();

	////裁剪几个截面
	////TODO 原图截面
	//m_ImageSamplerPtr->GetSampleMaskPlan(t_FrameDataPtr->m_pProstateMask, 0, 0);
	//m_ImageSamplerPtr->GetSampleMaskPlan(t_FrameDataPtr->m_pLesionMask, 0, 1);
	return;
}

/*****************************************************************
Name:			Mask2Edge
Inputs:
	Mat src - 待处理的mask图片 
Return Value:
	Mat - mask边缘
Description:	使用sobel算子 求mask的轮廓(强度为1)
*****************************************************************/
Mat AnalyseProcess::Mask2Edge(Mat src)
{
	int ddepth = CV_16S;
	int scale = 1, delta = 0;
	Mat res, grad_x, grad_y, grad;
	//求水平 竖直方向梯度
	Sobel(src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	Sobel(src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	//梯度转为绝对值
	convertScaleAbs(grad_x, grad_x);
	convertScaleAbs(grad_y, grad_y);
	//合并梯度
	addWeighted(grad_x, 1, grad_y, 1, 0, grad);
	//边缘强度置为1
	threshold(grad, res, 1, 1, 0);
	return res;
}