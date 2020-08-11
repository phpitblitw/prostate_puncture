//*****************************************************************
// 文件名 :						AnalyzeProcess.cpp
// 版本	 :						1.0
// 目的及主要功能 :				实际分析处理模块
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "stdafx.h"
#include "AnalyzeProcess.h"
#include "ErrorManager//ErrorCodeDefine.h"
#include "ComUtility/SysPathManager.h"
#include "ComUtility//IniFile.h"

using namespace fsutility;


///////////////////////////////////////////////////////
////////////////////////define/////////////////////////
///////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*****************************************************************
Global Variables
*****************************************************************/



/*****************************************************************
Function
*****************************************************************/
/*****************************************************************
Name:			AnalyzeThread
Inputs:
	ScanDeviceManagerPtr pDevice - 设备指针
Return Value:
	none
Description:	分析多线程
*****************************************************************/
DWORD WINAPI AnalyzeThread(LPVOID lpParam)			//分析线程
{
	AnalyzeProcess *t_pPoint;
	t_pPoint = (AnalyzeProcess *)lpParam;
	
	t_pPoint->Analyze();		//启动分析

	return 1;
}//AnalyzeThread



/*****************************************************************
Name:			AnalyzeProcess(待完成)
Inputs:
	none
Return Value:
	none
Description:	AnalyzeProcess构造函数
*****************************************************************/
AnalyzeProcess::AnalyzeProcess()
{
	m_SurgicalPlanPtr.reset(new SurgicalPlan());	//手术计划
	m_pMaskSampler.reset(new ImageSampler());		//掩模数据

	m_nShowImageX = 0;
	m_nShowImageY = 0;

	m_fMaxX = 0;
	m_fMaxY = 0;
	m_fMaxZ = 0;

	m_bStopAnalyze = false;

	m_NextFrameDataPtr.reset(new FrameData);
	m_CurrentFrameDataPtr = nullptr;

	//导入坐标变换矩阵
	CString t_strIniFileNamePath;
	t_strIniFileNamePath = CSysPathManager::Instance().GetConfigPath();
	t_strIniFileNamePath = t_strIniFileNamePath + _T("Analyze.ini");
	InportTransMatrixN_U(t_strIniFileNamePath);
}//AnalyzeProcess


/*****************************************************************
Name:			~AnalyzeProcess(待完成)
Inputs:
	none
Return Value:
	none
Description:	AnalyzeProcess析构函数
*****************************************************************/
AnalyzeProcess::~AnalyzeProcess()
{
}//~AnalyzeProcess


/*****************************************************************
Name:			InportTransMatrixN_U(待完成)
Inputs:
	none
Return Value:
	none
Description:	导入NDI——B超坐标转换矩阵
*****************************************************************/
int AnalyzeProcess::InportTransMatrixN_U(CString m_strFileName)
{
	//待添加
	//判断文件是否存在
	if (!PathFileExists(m_strFileName))
	{
		return ER_WrongAnalyzeConfigFilePathName;
	}

	fsutility::CIniFile t_ScanConfig;	//config文件

	if (!t_ScanConfig.Open(m_strFileName))
	{
		return ER_OpenUSBConfigFileFailed;
	}

	int i;
	for (i = 0; i < 6; ++i)
	{
		Mat t_Martix(4, 4, CV_32FC1);

		CString t_strSection;
		t_strSection.Format(_T("Martix%d"), i);
		t_ScanConfig.Move2Section(t_strSection);
		//t_ScanConfig.ReadKey(_T("00"), );


	}
	
	return LIST_NO_ERROR;
}


/*****************************************************************
Name:			BindUpdateFrameEvent
Inputs:
	none
Return Value:
	none
Description:	实际分析函数
*****************************************************************/
void AnalyzeProcess::BindUpdateFrameEvent(Fun_UpdateFrameEvent eventFun)
{
	m_UpdateFrameFun = eventFun;
}//BindUpdateFrameEvent


/*****************************************************************
Name:			SetNDIDevicePtr
Inputs:
	NDIOperatorPtr t_NDIOperatorPtr - NDI设备指针
Return Value:
	none
Description:	设置NDI设备操作指针
*****************************************************************/
void AnalyzeProcess::SetNDIDevicePtr(NDIOperatorPtr t_NDIOperatorPtr)
{
	m_NDIOperatorPtr = t_NDIOperatorPtr;

	m_NDIOperatorPtr->BindUpdateAttitudeEvent(std::bind(&AnalyzeProcess::UpdateNDIData, this, std::placeholders::_1));
}//SetNDIDevicePtr


/*****************************************************************
Name:			SetUSBCapturerPtr
Inputs:
	USBCapturerPtr t_USBCapturerPtr - 超声图像获取设备指针
Return Value:
	none
Description:	设置超声图像获取设备操作指针
*****************************************************************/
void AnalyzeProcess::SetUSBCapturerPtr(USBCapturerPtr t_USBCapturerPtr)
{
	m_USBCapturerPtr = t_USBCapturerPtr;

	m_USBCapturerPtr->BindCapturePerFrameEvent(std::bind(&AnalyzeProcess::UpdateUSBData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ));
}//SetUSBCapturerPtr


/*****************************************************************
Name:			Set2DImageSize
Inputs:
	int t_nWidth - 2D图像宽度
	int t_nHeight - 2D图像高度
Return Value:
	none
Description:	设置2D图像尺寸
*****************************************************************/
void AnalyzeProcess::Set2DImageSize(int t_nWidth, int t_nHeight)
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
Description:	设置2D图像尺寸
*****************************************************************/
void AnalyzeProcess::Set3DSize(float t_fMaxX, float t_fMaxY, float t_fMaxZ)
{
	if (t_fMaxX <= 50 || t_fMaxX > 1000
		|| t_fMaxY <= 50 || t_fMaxY > 1000
		|| t_fMaxZ <= 20 || t_fMaxZ > 1000 )
	{
		return;
	}

	m_fMaxX = t_fMaxX;
	m_fMaxY = t_fMaxY;
	m_fMaxZ = t_fMaxZ;
}//Set3DSize


/*****************************************************************
Name:			GetSurfaceDataPtr
Inputs:
	
Return Value:
	SurfaceDataPtr - 返回的面模型指针
Description:	返回表面模型
*****************************************************************/
SurfaceDataPtr AnalyzeProcess::GetSurfaceDataPtr()
{
	return m_SurgicalPlanPtr->GetSurfaceDataPtr();
}//GetSurfaceDataPtr


/*****************************************************************
Name:			GetCenterPt
Inputs:
	double &x - x坐标
	double &y - y坐标
	double &z - z坐标
Return Value:
	none
Description:	得到中心坐标
*****************************************************************/
void AnalyzeProcess::GetCenterPt(double &x, double &y, double &z)
{
	m_pMaskSampler->GetCenterPt(x, y, z);
}//GetCenterPt


/*****************************************************************
Name:			GetParameters
Inputs:
	（待填）
Return Value:
	FrameDataPtr - 导出的当前帧数据
Description:	导出当前一帧图像
*****************************************************************/
/*
void AnalyzeProcess::GetParameters(double &t_centerX, double &t_centerY, double &t_centerZ,
	double &t_MX, double &t_MY, double &t_MZ,
	double &t_UX, double &t_UY, double &t_UZ)
{
	return m_SurgicalPlanPtr->GetParameters(t_centerX, t_centerY, t_centerZ,
		t_MX, t_MY, t_MZ, t_UX, t_UY, t_UZ);
}//GetCurrentFrameData
*/


/*****************************************************************
Name:			InportData(待完成)
Inputs:
	none
Return Value:
	none
Description:	导入手术计划数据
*****************************************************************/
int AnalyzeProcess::InportData(CString t_strFilePathName)
{
	//判断文件是否存在
	if (!PathFileExists(t_strFilePathName))
	{
		return ER_FileOpenFailed;
	}

	//导入手术计划
	if (m_SurgicalPlanPtr->InPortAsFileSet(t_strFilePathName) != LIST_NO_ERROR)
	{
		return ER_InportSurgicalPlanFailed;
	}

	//关联ImageSamplerPtr
	m_pMaskSampler->SetDataPackage(m_SurgicalPlanPtr);

	return LIST_NO_ERROR;
}//InportData


/*****************************************************************
Name:			StartAnalyze
Inputs:
	none
Return Value:
	none
Description:	启动分析函数，创建分析线程
*****************************************************************/
void AnalyzeProcess::StartAnalyze()
{
	m_AnalyzeThread = CreateThread(nullptr, 0, AnalyzeThread, (void *)this, 0, nullptr);	//启动分析线程
}//StartAnalyze


/*****************************************************************
Name:			StartAnalyze(待完成)
Inputs:
	none
Return Value:
	none
Description:	结束分析函数，修改标志
*****************************************************************/
void AnalyzeProcess::StopAnalyze()
{
	m_USBCapturerPtr->StopGrab();			//停止采集图像数据
	m_NDIOperatorPtr->StopTracking();		//停止采集NDI数据
}


/*****************************************************************
Name:			Analyze(待完成)
Inputs:
	none
Return Value:
	none
Description:	实际分析函数
*****************************************************************/
void AnalyzeProcess::Analyze()
{
	while (1)
	{
		CSingleLock singlelock(&m_ProcessDataMutex);
		singlelock.Lock();
		if (!m_NextFrameDataPtr->m_USBImage.empty() && !m_NextFrameDataPtr->m_NeedlePos.Empty())
		{
			m_CurrentFrameDataPtr = m_NextFrameDataPtr;
			m_NextFrameDataPtr.reset(new FrameData);
			singlelock.Unlock();
		}
		else
		{
			Sleep(15);
			singlelock.Unlock();
			continue;
		}

		ProcessSingleFrame(m_CurrentFrameDataPtr);		//处理单帧数据
	}


}//Analyze


/*****************************************************************
Name:			ProcessSingleFrame
Inputs:
	none
Return Value:
	none
Description:	处理单帧数据
*****************************************************************/
void AnalyzeProcess::ProcessSingleFrame(FrameDataPtr t_FrameDataPtr)
{
	//调用次一级分析函数
	if (m_nAnalyzeState == Registration)
	{
		ProcessSingleFrameA(t_FrameDataPtr);
	}
	else
	{
		ProcessSingleFrameB(t_FrameDataPtr);
	}

	//将分析结果发送给dlg
	if (m_UpdateFrameFun != nullptr)
	{
		m_UpdateFrameFun(t_FrameDataPtr);
	}
}//ProcessSingleFrame


/*****************************************************************
Name:			ProcessSingleFrameA（待实现）
Inputs:
	none
Return Value:
	none
Description:	处理单帧数据（未关联状态）
*****************************************************************/
void AnalyzeProcess::ProcessSingleFrameA(FrameDataPtr t_FrameDataPtr)
{
	//得到第一帧2D图像
	if (t_FrameDataPtr->CreatMaskData(m_nShowImageX, m_nShowImageY) != LIST_NO_ERROR)
	{
		return;
	}

	m_pMaskSampler->GetSampleMaskPlan(t_FrameDataPtr->m_pMaskData, m_nShowImageX, m_nShowImageY,true);
}//ProcessSingleFrameA


/*****************************************************************
Name:			ProcessSingleFrameB
Inputs:
	none
Return Value:
	none
Description:	处理单帧数据（关联状态）
*****************************************************************/
void AnalyzeProcess::ProcessSingleFrameB(FrameDataPtr t_FrameDataPtr)
{
	//得到第一帧2D图像
	//得到第一帧2D图像
	if (t_FrameDataPtr->CreatMaskData(m_nShowImageX, m_nShowImageY) != LIST_NO_ERROR)
	{
		return;
	}

	m_pMaskSampler->GetSampleMaskPlan(t_FrameDataPtr->m_pMaskData, m_nShowImageX, m_nShowImageY,true);
}//ProcessSingleFrameB


/*****************************************************************
Name:			CalibratePlanUSBCoordinate(待完成)
Inputs:
	none
Return Value:
	none
Description:	计算超声探头与手术计划数据之间的转换矩阵
*****************************************************************/
void AnalyzeProcess::CalibratePlanUSBCoordinate()
{

}//CalibratePlanUSBCoordinate


/*****************************************************************
Name:			UpdateNDIData(待完成)
Inputs:
	none
Return Value:
	none
Description:	更新NDI数据，回调函数，由NDI模块调用
*****************************************************************/
void AnalyzeProcess::UpdateNDIData(std::vector<Attitude> t_Attitude)
{
	//现在默认只处理第一个
	if (t_Attitude.size() < 1)
	{
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	//先把Attitude保存下来

	CSingleLock singlelock(&m_ProcessDataMutex);
	singlelock.Lock();
	m_NextFrameDataPtr->m_USBPos = t_Attitude[0];
	singlelock.Unlock();
}//UpdateNDIData


/*****************************************************************
Name:			UpdateUSBData(Mat t_USBImg)
Inputs:
	cv::Mat t_USBImgA - 全图或上半区图像
	cv::Mat t_USBImgB - 下半区图像，可能为空
	int t_nScale - 缩放比例
Return Value:
	none
Description:	//更新USB数据，回调函数，由NDI模块调用
*****************************************************************/
void AnalyzeProcess::UpdateUSBData(cv::Mat t_USBImgA, cv::Mat t_USBImgB, int t_nScale)
{
	CSingleLock singlelock(&m_ProcessDataMutex);
	singlelock.Lock();
	m_NextFrameDataPtr->m_USBImage = t_USBImgA;
	m_NextFrameDataPtr->m_nScale = t_nScale;
	singlelock.Unlock();
}//UpdateUSBData


/*
void CALLBACK UpdateAttitudeCallBack(std::vector<NDIOPERATOR::Attitude> attitude)
{
	//HWND hWnd = AfxGetMainWnd()->m_hWnd;
	float x, y, z, a, b, c;

	//现在默认只处理第一个
	if (attitude.size() < 1)
	{
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	//先把Attitude保存下来

	CSingleLock singlelock(&pPuncturedlg->m_PosMutex);
	singlelock.Lock();
	pPuncturedlg->m_FrameDataPtr->m_USBPos = attitude[0];
	singlelock.Unlock();
	//double t_mat[16];
	//NDIOPERATOR::NDIOperator::ConstructMatRtoTusingAttitude(attitude[0], t_mat);

	//求得变换矩阵
	//CString str(""), temp;
	//for (int i = 0; i < 16; i++)
	//{
	//	temp.Format(_T("%.6f"), mat[i]);
	//	str += temp;
	//	str += CString(" ");
	//	if (i % 4 == 3)
	//		str += CString("\r\n");
	//}
	//GetDlgItem(IDC_EDIT_MatRtoT_1)->SetWindowText(str);
}



//void CALLBACK ProcessFrameCallBack(cv::Mat t_USBImgA, cv::Mat t_USBImgB, int t_nScale)
//{
//	//开始处理图片，截取一部分图片出来,先转换成bitmap
//
//	CSingleLock singlelock(&pPuncturedlg->m_ShowMutex);
//	singlelock.Lock();
//
//	//pPuncturedlg->m_FrameDataPtr->m_USBImage = t_USBImgA;
//	//pPuncturedlg->m_FrameDataPtr->m_nScale = t_nScale;
//	//pPuncturedlg->m_b2Dshow = true;
//
//	singlelock.Unlock();
//}*/