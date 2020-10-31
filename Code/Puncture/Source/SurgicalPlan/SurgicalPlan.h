//*****************************************************************
// 文件名 :						SurgicalPlan.h
// 版本	 :						1.0
// 目的及主要功能 :				手术计划类
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
#include <vector>
#include <fstream>
#include <string>
#include "Opencv2/opencv.hpp"
#include "Target.h"
#include "NDIOperator/Attitude.h"
#include "MaskData.h"
#include "MRIData.h"
#include "SurfaceData.h"
#include "CPubFunc.h"
//#include "ImageSampler.h"

using namespace std;
using namespace NDIOPERATOR;

/*****************************************************************
Defines
*****************************************************************/
#ifdef SURGICALPLAN_EXPORT
#define SURGICALPLAN_API __declspec(dllexport)
#else
#define SURGICALPLAN_API __declspec(dllimport)
#endif


/*****************************************************************
Routine Definitions

手术计划管理模块，用于前端软件与导航软件传递数据，术中提供相关信息截图
定义SurgicalPlan类，该类包含：
数据
i.	MRI数据（裸数据，uchar数组）；
ii.	MRI基本参数（像素分辨率、像素数、病人信息等，待扩充）；
iii.	分割后目标区域掩膜；
vii.	前列腺预设靶点数据；（定义新的类TargetPoint类，该类包含了空间坐标及预设进针路径）
viii.	直肠轴线设定功能，用两个点代表直肠中轴线
功能
ix.	导入DICOM数据为MRI数据功能；
x.	设置单层轮廓数据功能；
xi.	由单层轮廓数据生成三角形网格功能；
xii.	由癌变区域轮廓数据生成三角形网格功能；
xiii.	设置前列腺靶点功能；
xiv.	数据导出为文件组功能；
xv.	文件组导入为数据功能；
xvi.	给定坐标截取平面，返回轮廓线功能；
xvii.	初始化及析构功能。

*****************************************************************/

namespace SURGICALPLAN
{
	//声明ImageSampler类
	//class ImageSampler;
	//typedef std::shared_ptr<SURGICALPLAN::ImageSampler> ImageSamplerPtr;
	//定义手术计划类
	class SURGICALPLAN_API SurgicalPlan
	{
	public:
		SurgicalPlan();
		virtual ~SurgicalPlan();

		int ReadDICOMData(CString t_strDICOMFilePathName);		//读取DICOM文件至m_pMRIDate，并初始化各参数
		int CountProstateMask();								//计算前列腺三维掩膜

		int ExPortAsFileSet(CString t_strPlanFilePath);					//将手术计划导出为文件
		int InPortAsFileSet(CString t_strFilePathName);					//导入手术计划文件
		int InPortAsFileSet(std::string t_strFilePathName);				//导入手术计划文件
		//int GetOutline(cv::Mat &t_OutlineImg, Attitude t_Attitude);		//根据平面截取轮廓图像

		MRIDataPtr GetMRIDataPtr() { return m_pMRIData; }				//获取MRI信息
		MaskDataPtr GetMaskDataPtr(int nMaskType);						//获取Mask信息
		SurfaceDataPtr GetSurfaceDataPtr() { return m_pSurface; }		//获取表面信息
		

		void SetImageSize(int nImageX, int nImageY); //设置2d显示的超声尺寸(像素数)

	protected:
		void InitData();
		void DestroyData();

	private:
		MRIDataPtr m_pMRIData;							//MRI裸数据
		MaskDataPtr m_pProstateMaskData;				//前列腺Mask数据
		MaskDataPtr m_pLesionMaskData;					//病灶Mask数据
		MaskDataPtr m_pRectumMaskData;					//直肠Mask数据

		SurfaceDataPtr m_pSurface;						//表面标签数据(目前为prostate标签)

		vector <Point3D> m_vRectumCenterPath;			//直肠中心点序列
		int m_iCurCenterPtIndex;						//当前直肠点在序列中的位置
		int m_iBaseCenterPtIndex;						//用于配准的平面对应的直肠中心点下标

		//mr数据尺寸(像素数)
		int m_nSizeX;
		int m_nSizeY;
		int m_nSizeZ;

		//2d显示的超声尺寸(像素数)
		int m_nShowImageX;
		int m_nShowImageY;

		//mr体素大小
		float m_fVoxelSizeX;
		float m_fVoxelSizeY;
		float m_fVoxelSizeZ;

		CString m_strRootPath;
		CString m_strMRIFileName;
		//CString m_strMaskFileName;
		CString m_strProstateMaskFileName;
		CString m_strLesionMaskFileName;
		CString m_strRectumMaskFileName;
		//CString m_strSurfaceFileName;	
		//CString m_strRectumCenterName;
		CString m_strProstateObjFileName;
		CString m_strLesionObjFileName;
		CString m_strRectumObjFileName;

		void AddRectumCenterPt(double x, double y, double z); //添加一个直肠中心点坐标，序列数据通过该函数一个个添加
		Point3D GetCurRectumPt(); //得到当前的直肠中心点
		Point3D GetCurRectumDir(); //得到当前中心点对应方向
		int GoAhead(); //当前直肠点 下标+1
	};

	//定义智能指针类
	typedef std::shared_ptr<SurgicalPlan> SurgicalPlanPtr;
	
}


























//// SurgicalPlan.h: SurgicalPlan DLL 的主标头文件
////
//
//#pragma once
//
//#ifndef __AFXWIN_H__
//#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
//#endif
//
//#include "resource.h"		// 主符号
//
//
//// CSurgicalPlanApp
//// 有关此类实现的信息，请参阅 SurgicalPlan.cpp
////
//
//class CSurgicalPlanApp : public CWinApp
//{
//public:
//	CSurgicalPlanApp();
//
//	// 重写
//public:
//	virtual BOOL InitInstance();
//
//	DECLARE_MESSAGE_MAP()
//};
