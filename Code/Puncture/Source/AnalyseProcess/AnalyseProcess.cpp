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
//DWORD WINAPI AnalyseThread(LPVOID lpParam)			//分析线程
//{
//	AnalyseProcess *t_pPoint;
//	t_pPoint = (AnalyseProcess *)lpParam;
//
//	t_pPoint->Analyse();		//启动分析
//
//	return 1;
//}//AnalyzeThread


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
	m_bAnalysing = false;
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
	this->StopAnalyse();
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

int AnalyseProcess::InitAnalyseProcess(std::string t_strFilePathName)
{
	CString t_cstrFilePathName(t_strFilePathName.data());
	return InitAnalyseProcess(t_cstrFilePathName);
}


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

	m_USBCapturerPtr->BindCapturePerFrameEvent(std::bind(&AnalyseProcess::UpdateUSBData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
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
int AnalyseProcess::StartAnalyse()
{
	//m_hAnalyzeThread = CreateThread(nullptr, 0, AnalyseThread, this, 0, nullptr);
	if (m_bAnalysing)
		return ER_InitAnalyseProcessFailed;

	//开辟分析线程
	m_bAnalysing = true;
	m_tAnalyseThread = thread(bind(&AnalyseProcess::Analyse, this));
	m_tAnalyseThread.detach();
	return LIST_NO_ERROR;
}//StartAnalyze


/*****************************************************************
Name:			StopAnalyze
Inputs:
	none
Return Value:
	none
Description:	结束分析函数，修改标志
*****************************************************************/
int AnalyseProcess::StopAnalyse()
{
	if (m_USBCapturerPtr->StopGrab() != LIST_NO_ERROR)
		return ER_CloseAnalyseProcessFailed;
	if (m_NDIOperatorPtr->StopTracking() != LIST_NO_ERROR)
		return ER_CloseAnalyseProcessFailed;
	if (m_bAnalysing == false)
		return ER_CloseAnalyseProcessFailed;

	m_bAnalysing = false;
	m_nAnalyseState = Init;
	return LIST_NO_ERROR;
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
	while (m_bAnalysing)
	{
		CSingleLock singlelock(&m_ProcessDataMutex);
		singlelock.Lock();
		//如果有待处理的，来自B超采集模块的图像数据，则计算对应的MRI模拟采样，生成对应的FrameData
		if(m_NextFrameDataPtr->m_USImgT.data || m_NextFrameDataPtr->m_USImgS.data)
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
	m_PositionManagerPtr->m_BaseMRIAttitude = AnalyseConfig::Instance().m_Attitude;
	//将当前超声位置作为超声base位置，记录在PositionManager中
	singlelock.Lock();
	m_PositionManagerPtr->m_BaseUSAttitude = m_PositionManagerPtr->m_CurUSAttitude;
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
void AnalyseProcess::UpdateNDIData(fsutility::Attitude attitude)
{
	CSingleLock singlelock(&m_ProcessDataMutex);
	singlelock.Lock();
	m_PositionManagerPtr->m_CurUSAttitude = attitude;
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
//void AnalyseProcess::UpdateUSBData(cv::Mat t_USBImgA, cv::Mat t_USBImgB, double m_dImageRes)
//{
//	CSingleLock singlelock(&m_ProcessDataMutex);
//	singlelock.Lock();
//	m_NextFrameDataPtr->m_USBImage = t_USBImgA;
//	m_NextFrameDataPtr->m_dImageRes = m_dImageRes;
//	
//	m_ImageSamplerPtr->SetUSPixelSize(m_dImageRes);	//设置B超图像像素大小(一个像素的物理尺寸)
//	m_ImageSamplerPtr->SetImageSize(t_USBImgA.cols, t_USBImgA.rows);	//设置B超图像大小(像素数)
//	singlelock.Unlock();
//}//UpdateUSBData

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
void AnalyseProcess::UpdateUSBData(cv::Mat t_USBImgT, cv::Mat t_USBImgS, double dPixelSizeT, double dPixelSizeS)
{
	CSingleLock singlelock(&m_ProcessDataMutex);
	singlelock.Lock();

	m_NextFrameDataPtr->m_USImgT = t_USBImgT;
	m_NextFrameDataPtr->m_USImgS = t_USBImgS;
	m_NextFrameDataPtr->m_dPixelSizeT = dPixelSizeT;
	m_NextFrameDataPtr->m_dPixelSizeS = dPixelSizeS;
	
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

	int width, height;
	//处理横断面Transverse
	if (!t_FrameDataPtr->m_USImgT.empty())
	{
		width = t_FrameDataPtr->m_USImgT.cols;
		height = t_FrameDataPtr->m_USImgT.rows;
		//m_ImageSamplerPtr->SetUSPixelSize(t_FrameDataPtr->m_dPixelSizeT);
		//m_ImageSamplerPtr->SetImageSize(width, height);
		m_pProstateMask = new BYTE[width * height];
		m_pLesionMask = new BYTE[width * height];
		m_pRectumMask = new BYTE[width * height];
		memset(m_pProstateMask, 0, sizeof(BYTE)*width*height);
		memset(m_pLesionMask, 0, sizeof(BYTE)*width*height);
		memset(m_pRectumMask, 0, sizeof(BYTE)*width*height);
		//m_ImageSamplerPtr->GetSampleMaskPlan(m_pProstateMask, 0, 1);
		//m_ImageSamplerPtr->GetSampleMaskPlan(m_pLesionMask, 0, 2);
		//m_ImageSamplerPtr->GetSampleMaskPlan(m_pRectumMask, 0, 3);
		Mat prostateMask(height, width, CV_8UC1, m_pProstateMask);
		Mat lesionMask(height, width, CV_8UC1, m_pLesionMask);
		Mat rectumMask(height, width, CV_8UC1, m_pRectumMask);
		t_FrameDataPtr->m_prostateMaskT = prostateMask.clone();
		t_FrameDataPtr->m_lesionMaskT = lesionMask.clone();
		t_FrameDataPtr->m_rectumMaskT = rectumMask.clone();
		delete[] m_pProstateMask;
		delete[] m_pLesionMask;
		delete[] m_pRectumMask;
	}

	////开辟mask空间
	//singlelock.Lock();
	//if (t_FrameDataPtr->CreatMaskData(m_nShowImageX, m_nShowImageY) != LIST_NO_ERROR)
	//{
	//	return;
	//}
	//未关联状态，无需进行MRI模拟采样，将MASK直接置为全0
	//TODO，设置空的MRI原始数据截面
	//memset(t_FrameDataPtr->m_pFusionMask, 0, sizeof(BYTE)*m_nShowImageX*m_nShowImageY);
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
	singlelock.Lock();

	//计算当前截面位置
	m_PositionManagerPtr->UpDate();	//根据已经获取的超声探头位置参数，更新MRI模拟采样位置参数
	m_ImageSamplerPtr->SetPosition(m_PositionManagerPtr->m_CurMRIAttitude);	//为ImageSampler设置位置参数
	t_FrameDataPtr->SetPosition(m_PositionManagerPtr->m_CurMRIAttitude);  //将当前位置参数交付FrameDataPtr

	//更新当前截面4个角点的位置(wld)
	//m_ImageSamplerPtr->GetPlaneCorners(t_FrameDataPtr->m_LeftTop, t_FrameDataPtr->m_RightTop, t_FrameDataPtr->m_LeftBottom, t_FrameDataPtr->m_RightBottom);

	int width, height;
	//处理横断面Transverse
	if (!t_FrameDataPtr->m_USImgT.empty())
	{
		width = t_FrameDataPtr->m_USImgT.cols;
		height = t_FrameDataPtr->m_USImgT.rows;
		m_ImageSamplerPtr->SetUSPixelSize(t_FrameDataPtr->m_dPixelSizeT);
		m_ImageSamplerPtr->SetImageSize(width, height);
		m_pProstateMask = new BYTE[width * height];
		m_pLesionMask = new BYTE[width * height];
		m_pRectumMask = new BYTE[width * height];
		m_ImageSamplerPtr->GetSampleMaskPlan(m_pProstateMask, 0, 1);
		m_ImageSamplerPtr->GetSampleMaskPlan(m_pLesionMask, 0, 2);
		m_ImageSamplerPtr->GetSampleMaskPlan(m_pRectumMask, 0, 3);
		Mat prostateMask(height, width, CV_8UC1, m_pProstateMask);
		Mat lesionMask(height, width, CV_8UC1, m_pLesionMask);
		Mat rectumMask(height, width, CV_8UC1, m_pRectumMask);
		t_FrameDataPtr->m_prostateMaskT = prostateMask.clone();
		t_FrameDataPtr->m_lesionMaskT = lesionMask.clone();
		t_FrameDataPtr->m_rectumMaskT = rectumMask.clone();
		delete[] m_pProstateMask;
		delete[] m_pLesionMask;
		delete[] m_pRectumMask;
	}
	//处理矢状面Sagittal
	if (!t_FrameDataPtr->m_USImgS.empty())
	{
		width = t_FrameDataPtr->m_USImgS.cols;
		height = t_FrameDataPtr->m_USImgS.rows;
		m_ImageSamplerPtr->SetUSPixelSize(t_FrameDataPtr->m_dPixelSizeS);
		m_ImageSamplerPtr->SetImageSize(width, height);
		m_pProstateMask = new BYTE[width * height];
		m_pLesionMask = new BYTE[width * height];
		m_pRectumMask = new BYTE[width * height];
		m_ImageSamplerPtr->GetSampleMaskPlan(m_pProstateMask, 1, 1);
		m_ImageSamplerPtr->GetSampleMaskPlan(m_pLesionMask, 1, 2);
		m_ImageSamplerPtr->GetSampleMaskPlan(m_pRectumMask, 1, 3);
		Mat prostateMask(height, width, CV_8UC1, m_pProstateMask);
		Mat lesionMask(height, width, CV_8UC1, m_pLesionMask);
		Mat rectumMask(height, width, CV_8UC1, m_pRectumMask);
		t_FrameDataPtr->m_prostateMaskS = prostateMask.clone();
		t_FrameDataPtr->m_lesionMaskS = lesionMask.clone();
		t_FrameDataPtr->m_rectumMaskS = rectumMask.clone();
		delete[] m_pProstateMask;
		delete[] m_pLesionMask;
		delete[] m_pRectumMask;
	}

	singlelock.Unlock();
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